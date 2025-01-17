/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"
#include "engines/util.h"
#include "graphics/scaler.h"
#include "graphics/thumbnail.h"
#include "glk/glk.h"
#include "glk/blorb.h"
#include "glk/conf.h"
#include "glk/events.h"
#include "glk/picture.h"
#include "glk/quetzal.h"
#include "glk/screen.h"
#include "glk/selection.h"
#include "glk/sound.h"
#include "glk/streams.h"
#include "glk/windows.h"

namespace Glk {

GlkEngine *g_vm;

GlkEngine::GlkEngine(OSystem *syst, const GlkGameDescription &gameDesc) :
		_gameDescription(gameDesc), Engine(syst), _random("Glk"), _blorb(nullptr),
		_clipboard(nullptr), _conf(nullptr), _events(nullptr), _pictures(nullptr),
		_screen(nullptr), _selection(nullptr), _sounds(nullptr), _windows(nullptr),
		_copySelect(false), _terminated(false), _pcSpeaker(nullptr),
		gli_register_obj(nullptr), gli_unregister_obj(nullptr), gli_register_arr(nullptr),
		gli_unregister_arr(nullptr) {
	// Set up debug channels
	DebugMan.addDebugChannel(kDebugCore, "core", "Core engine debug level");
	DebugMan.addDebugChannel(kDebugScripts, "scripts", "Game scripts");
	DebugMan.addDebugChannel(kDebugGraphics, "graphics", "Graphics handling");
	DebugMan.addDebugChannel(kDebugSound, "sound", "Sound and Music handling");

	g_vm = this;
}

GlkEngine::~GlkEngine() {
	delete _blorb;
	delete _clipboard;
	delete _conf;
	delete _events;
	delete _pcSpeaker;
	delete _pictures;
	delete _screen;
	delete _selection;
	delete _sounds;
	delete _streams;
	delete _windows;
}

void GlkEngine::initialize() {
	initGraphicsMode();

	_conf = new Conf(getInterpreterType());
	_screen = createScreen();
	_screen->initialize();
	_clipboard = new Clipboard();
	_events = new Events();
	_pcSpeaker = new PCSpeaker(_mixer);
	_pictures = new Pictures();
	_selection = new Selection();
	_sounds = new Sounds();
	_streams = new Streams();
	_windows = new Windows(_screen);
}

Screen *GlkEngine::createScreen() {
	return new Screen();
}

void GlkEngine::initGraphicsMode() {
	uint width = ConfMan.hasKey("width") ? ConfMan.getInt("width") : 640;
	uint height = ConfMan.hasKey("height") ? ConfMan.getInt("height") : 480;
	Common::List<Graphics::PixelFormat> formats = g_system->getSupportedFormats();
	Graphics::PixelFormat format = formats.front();

	for (Common::List<Graphics::PixelFormat>::iterator i = formats.begin(); i != formats.end(); ++i) {
		if ((*i).bytesPerPixel > 1) {
			format = *i;
			break;
		}
	}

	initGraphics(width, height, &format);
}

Common::Error GlkEngine::run() {
	// Open up the game file
	Common::String filename = getFilename();
	if (!Common::File::exists(filename))
		return Common::kNoGameDataFoundError;

	if (Blorb::isBlorb(filename)) {
		// Blorb archive
		_blorb = new Blorb(filename, getInterpreterType());
		SearchMan.add("blorb", _blorb, 99, false);

		if (!_gameFile.open("game", *_blorb))
			return Common::kNoGameDataFoundError;
	} else {
		// Check for a secondary blorb file with the same filename
		Common::String baseName = filename;
		while (baseName.contains('.'))
			baseName.deleteLastChar();

		if (Common::File::exists(baseName + ".blorb")) {
			_blorb = new Blorb(baseName + ".blorb", getInterpreterType());
			SearchMan.add("blorb", _blorb, 99, false);
		} else if (Common::File::exists(baseName + ".blb")) {
			_blorb = new Blorb(baseName + ".blb", getInterpreterType());
			SearchMan.add("blorb", _blorb, 99, false);
		}

		// Open up the game file
		if (!_gameFile.open(filename))
			return Common::kNoGameDataFoundError;
	}

	// Perform initialization
	initialize();

	// Play the game
	runGame();

	return Common::kNoError;
}

Common::Error GlkEngine::loadGame() {
	frefid_t ref = _streams->createByPrompt(fileusage_BinaryMode | fileusage_SavedGame,
		filemode_Read, 0);
	if (ref == nullptr)
		return Common::kReadingFailed;

	int slotNumber = ref->_slotNumber;
	_streams->deleteRef(ref);

	return loadGameState(slotNumber);
}

Common::Error GlkEngine::saveGame() {
	frefid_t ref = _streams->createByPrompt(fileusage_BinaryMode | fileusage_SavedGame,
		filemode_Write, 0);
	if (ref == nullptr)
		return Common::kWritingFailed;

	int slot = ref->_slotNumber;
	Common::String desc = ref->_description;
	_streams->deleteRef(ref);

	return saveGameState(slot, desc);
}

Common::Error GlkEngine::loadGameState(int slot) {
	FileReference ref(slot, "", fileusage_SavedGame | fileusage_TextMode);

	strid_t file = _streams->openFileStream(&ref, filemode_Read);
	if (file == nullptr)
		return Common::kReadingFailed;

	Common::ErrorCode errCode = Common::kNoError;
	QuetzalReader r;
	if (r.open(*file, ID_IFSF)) {
		// First scan for a SCVM chunk. It has information of the game the save is for,
		// so if present we can validate the save is for this game
		for (QuetzalReader::Iterator it = r.begin(); it != r.end(); ++it) {
			if ((*it)._id == ID_SCVM) {
				// Skip over date/time & playtime
				Common::SeekableReadStream *rs = it.getStream();
				rs->skip(14);

				byte interpType = rs->readByte();
				byte language = rs->readByte();
				Common::String md5 = QuetzalReader::readString(rs);
				delete rs;

				if (interpType != getInterpreterType() || language != getLanguage() || md5 != getGameMD5())
					errCode = Common::kReadingFailed;
			}
		}

		if (errCode == Common::kNoError) {
			// Scan for an uncompressed memory chunk
			errCode = Common::kReadingFailed;		// Presume we won't find chunk
			for (QuetzalReader::Iterator it = r.begin(); it != r.end(); ++it) {
				if ((*it)._id == ID_UMem) {
					Common::SeekableReadStream *rs = it.getStream();
					errCode = readSaveData(rs).getCode();
					delete rs;
					break;
				}
			}
		}
	}

	file->close();
	return errCode;
}

Common::Error GlkEngine::saveGameState(int slot, const Common::String &desc) {
	Common::String msg;
	FileReference ref(slot, desc, fileusage_BinaryMode | fileusage_SavedGame);

	strid_t file = _streams->openFileStream(&ref, filemode_Write);
	if (file == nullptr)
		return Common::kWritingFailed;

	Common::ErrorCode errCode = Common::kNoError;
	QuetzalWriter w;

	// Add the uncompressed memory chunk with the game's save data
	{
		Common::WriteStream &ws = w.add(ID_UMem);
		errCode = writeGameData(&ws).getCode();
	}

	if (errCode == Common::kNoError) {
		w.save(*file, desc);
	}

	file->close();
	return errCode;
}

void GlkEngine::beep() {
	_pcSpeaker->speakerOn(50, 50);
}

} // End of namespace Glk

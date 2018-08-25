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

#ifndef ULTIMA_H
#define ULTIMA_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/endian.h"
#include "common/hash-str.h"
#include "common/serializer.h"
#include "common/random.h"
#include "common/savefile.h"
#include "common/util.h"
#include "graphics/surface.h"
#include "engines/engine.h"

namespace Ultima {

enum {
	kDebugLevelScript      = 1 << 0
};

enum GameType {
	GType_Ultima1	= 0
};

enum UltimaGameFeatures {
	GF_VGA_ENHANCED = 1 << 0
};

struct UltimaGameDescription;

struct UltimaSavegameHeader {
	uint8 _version;
	uint8 _gameId;
	uint8 _language;
	uint8 _videoMode;
	Common::String _saveName;
	Graphics::Surface *_thumbnail;
	int _year, _month, _day;
	int _hour, _minute;
	int _totalFrames;
};

class Debugger;
class Events;
class GameBase;
class MouseCursor;
class Resources;

namespace Gfx {
	class Screen;
}
namespace Shared {
	class Game;
}

class UltimaEngine : public Engine {
private:
	Common::RandomSource _randomSource;
private:
	/**
	 * Initialize the engine
	 */
	bool initialize();

	/**
	 * Deinitialize the engine
	 */
	void deinitialize();
public:
	const UltimaGameDescription *_gameDescription;
	Debugger *_debugger;
	Events *_events;
	GameBase *_game;
	MouseCursor *_mouseCursor;
	Gfx::Screen *_screen;
public:
	UltimaEngine(OSystem *syst, const UltimaGameDescription *gameDesc);
	virtual ~UltimaEngine();

	/**
	 * Main method for running the game
	 */
	virtual Common::Error run() override;

	/**
	 * Play the game
	 */
	void playGame();

	/**
	 * Returns the Id of the game
	 */
	GameType getGameID() const;

	/**
	 * Return the game's language
	 */
	Common::Language getLanguage() const;

	/**
	 * Return the game's features
	 */
	uint32 getFeatures() const;

	/**
	 * Return if the game is running in the new VGA enhanced mode
	 */
	bool isVGAEnhanced() const;

	/**
	 * Creates a new hierarchy for the game, that contains all the logic for playing that particular game.
	 */
	Shared::Game *createGame() const;

	/**
	 * Gets a random number
	 */
	uint getRandomNumber(uint max) { return _randomSource.getRandomNumber(max); }

	/**
	 * Gets a random number
	 */
	uint getRandomNumber(uint min, uint max) { return min + _randomSource.getRandomNumber(max - min); }

	/**
	 * Displays an error message in a GUI dialog
	 */
	void GUIError(const char *msg, ...) GCC_PRINTF(2, 3);

	/**
	 * Load a savegame
	 */
	virtual Common::Error loadGameState(int slot) override;

	/**
	 * Save the game
	 */
	virtual Common::Error saveGameState(int slot, const Common::String &desc) override;

	/**
	 * Returns true if a savegame can currently be loaded
	 */
	virtual bool canLoadGameStateCurrently() override;

	/**
	 * Returns true if the game can currently be saved
	 */
	virtual bool canSaveGameStateCurrently() override;

	/**
	 * Get engine features
	 */
	virtual bool hasFeature(EngineFeature f) const override;

	/**
	 * Load a pending save
	 */
	bool loadGame();

	/**
	 * Save the game
	 */
	bool saveGame();

	/**
	 * Read in a savegame header
	 */
	static bool readSavegameHeader(Common::InSaveFile *in, UltimaSavegameHeader &header, bool skipThumbnail);

	/**
	 * Write out a savegame header
	 */
	void writeSavegameHeader(Common::OutSaveFile *out, const Common::String &saveName);
};

extern UltimaEngine *g_vm;

} // End of namespace Ultima

#endif

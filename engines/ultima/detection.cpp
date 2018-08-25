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

#include "ultima/ultima.h"
#include "ultima/events.h"
#include "common/system.h"
#include "common/translation.h"
#include "engines/advancedDetector.h"

namespace Ultima {

#define MAX_SAVES 99

struct UltimaGameDescription {
	ADGameDescription desc;

	GameType gameID;
	uint32 features;
};

GameType UltimaEngine::getGameID() const {
	return _gameDescription->gameID;
}

Common::Language UltimaEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

uint32 UltimaEngine::getFeatures() const {
	return _gameDescription->features;
}

bool UltimaEngine::isVGAEnhanced() const {
	return getFeatures() & GF_VGA_ENHANCED;
}

} // End of namespace Ultima

static const PlainGameDescriptor ultimaGames[] = {
	{ "ultima1", "Ultima 1: The First Age of Darkness" },
	{0, 0}
};

#include "ultima/detection_tables.h"

class UltimaMetaEngine : public AdvancedMetaEngine {
public:
	UltimaMetaEngine() : AdvancedMetaEngine(Ultima::gameDescriptions, sizeof(Ultima::UltimaGameDescription),
		ultimaGames) {}

	virtual const char *getName() const {
		return "Ultima I Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Ultima Engine (C) Origin";
	}

	/**
	 * Creates an instance of the game engine
	 */
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

	/**
	 * Returns a list of features the game's MetaEngine support
	 */
	virtual bool hasFeature(MetaEngineFeature f) const;

	virtual SaveStateList listSaves(const char *target) const override;
	virtual int getMaximumSaveSlot() const override;
	virtual void removeSaveState(const char *target, int slot) const override;
	virtual SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const override;
};

bool UltimaMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
		const Ultima::UltimaGameDescription *gd = (const Ultima::UltimaGameDescription *)desc;
	*engine = new Ultima::UltimaEngine(syst, gd);

	return gd != 0;
}

bool UltimaMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportCreationDate) ||
		(f == kSavesSupportPlayTime) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSimpleSavesNames);
}

bool Ultima::UltimaEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

SaveStateList UltimaMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String saveDesc;
	Common::String pattern = Common::String::format("%s.###", target);
	Ultima::UltimaSavegameHeader header;

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		const char *ext = strrchr(file->c_str(), '.');
		int slot = ext ? atoi(ext + 1) : -1;

		if (slot >= 0 && slot <= MAX_SAVES) {
			Common::InSaveFile *in = g_system->getSavefileManager()->openForLoading(*file);

			if (in) {
				if (Ultima::UltimaEngine::readSavegameHeader(in, header, true))
					saveList.push_back(SaveStateDescriptor(slot, header._saveName));

				delete in;
			}
		}
	}

	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}

int UltimaMetaEngine::getMaximumSaveSlot() const {
	return MAX_SAVES;
}

void UltimaMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s.%03d", target, slot);
	g_system->getSavefileManager()->removeSavefile(filename);
}

SaveStateDescriptor UltimaMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s.%03d", target, slot);
	Common::InSaveFile *f = g_system->getSavefileManager()->openForLoading(filename);

	if (f) {
		Ultima::UltimaSavegameHeader header;
		if (!Ultima::UltimaEngine::readSavegameHeader(f, header, false)) {
			delete f;
			return SaveStateDescriptor();
		}

		delete f;

		// Create the return descriptor
		SaveStateDescriptor desc(slot, header._saveName);
		desc.setThumbnail(header._thumbnail);
		desc.setSaveDate(header._year, header._month, header._day);
		desc.setSaveTime(header._hour, header._minute);
		desc.setPlayTime(header._totalFrames * GAME_FRAME_TIME);

		return desc;
	}

	return SaveStateDescriptor();
}

#if PLUGIN_ENABLED_DYNAMIC(ULTIMA)
	REGISTER_PLUGIN_DYNAMIC(ULTIMA, PLUGIN_TYPE_ENGINE, UltimaMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(ULTIMA, PLUGIN_TYPE_ENGINE, UltimaMetaEngine);
#endif

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
#include "common/system.h"
#include "common/translation.h"
#include "engines/advancedDetector.h"

namespace Ultima {

struct UltimaGameDescription {
	ADGameDescription desc;

	GameType gameID;
};

GameType UltimaEngine::getGameID() const {
	return _gameDescription->gameID;
}

Common::Language UltimaEngine::getLanguage() const {
	return _gameDescription->desc.language;
}


} // End of namespace Ultima

static const PlainGameDescriptor ultimaGames[] = {
	{ "ultima1", "Ultima I - The First Age of Darkness" },
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
};

bool UltimaMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
		const Ultima::UltimaGameDescription *gd = (const Ultima::UltimaGameDescription *)desc;
	*engine = new Ultima::UltimaEngine(syst, gd);

	return gd != 0;
}

bool UltimaMetaEngine::hasFeature(MetaEngineFeature f) const {
	return 0;
}

#if PLUGIN_ENABLED_DYNAMIC(ULTIMA)
	REGISTER_PLUGIN_DYNAMIC(ULTIMA, PLUGIN_TYPE_ENGINE, UltimaMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(ULTIMA, PLUGIN_TYPE_ENGINE, UltimaMetaEngine);
#endif

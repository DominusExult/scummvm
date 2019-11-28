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

#include "ags/ags.h"
#include "ags/detection.h"
#include "base/plugins.h"
#include "common/savefile.h"
#include "common/str-array.h"
#include "common/memstream.h"
#include "common/system.h"
#include "common/translation.h"
#include "graphics/colormasks.h"
#include "graphics/surface.h"

static const PlainGameDescriptor agsGames[] = {
	{ "ags", "Adventure Game Studio" },
	{0, 0}
};

#include "ags/detection_tables.h"

AGSMetaEngine::AGSMetaEngine() : AdvancedMetaEngine(AGS::gameDescriptions,
	sizeof(AGS::AGSGameDescription), agsGames) {
}

bool AGSMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSimpleSavesNames);
}

bool AGS::AGSEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

bool AGSMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const AGS::AGSGameDescription *gd = (const AGS::AGSGameDescription *)desc;
	if (gd) {
		*engine = new AGS::AGSEngine(syst, gd);
	}
	return gd != 0;
}

SaveStateList AGSMetaEngine::listSaves(const char *target) const {
	SaveStateList saveList;
	return saveList;
}

int AGSMetaEngine::getMaximumSaveSlot() const {
	return MAX_SAVES;
}

void AGSMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s.%03d", target, slot);
	g_system->getSavefileManager()->removeSavefile(filename);
}

SaveStateDescriptor AGSMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	return SaveStateDescriptor();
}

#if PLUGIN_ENABLED_DYNAMIC(AGS)
	REGISTER_PLUGIN_DYNAMIC(AGS, PLUGIN_TYPE_ENGINE, AGSMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(AGS, PLUGIN_TYPE_ENGINE, AGSMetaEngine);
#endif

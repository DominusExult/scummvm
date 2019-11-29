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
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"

#include "ags/common/ac/audiocliptype.h"
#include "ags/common/ac/characterinfo.h"
#include "ags/common/ac/common.h"
#include "ags/common/ac/common_defines.h"
#include "ags/common/ac/dialogtopic.h"
#include "ags/common/ac/gamesetupstruct.h"
#include "ags/common/ac/gamesetupstructbase.h"
#include "ags/common/ac/gamestructdefines.h"
#include "ags/common/ac/game_version.h"
#include "ags/common/ac/interfacebutton.h"
#include "ags/common/ac/interfaceelement.h"
#include "ags/common/ac/inventoryiteminfo.h"
#include "ags/common/ac/mousecursor.h"
#include "ags/common/ac/oldgamesetupstruct.h"
#include "ags/common/ac/spritecache.h"
#include "ags/common/ac/view.h"
#include "ags/common/ac/wordsdictionary.h"

#include "ags/common/core/asset.h"
#include "ags/common/core/assetmanager.h"
#include "ags/common/core/def_version.h"
#include "ags/common/core/platform.h"
#include "ags/common/core/types.h"

#include "ags/common/debug/assert.h"
#include "ags/common/debug/debugmanager.h"
#include "ags/common/debug/out.h"
#include "ags/common/debug/outputhandler.h"

#include "ags/common/font/agsfontrenderer.h"
#include "ags/common/font/fonts.h"
#include "ags/common/font/ttffontrenderer.h"
#include "ags/common/font/wfnfont.h"
#include "ags/common/font/wfnfontrenderer.h"

#include "ags/common/game/customproperties.h"
#include "ags/common/game/interactions.h"
#include "ags/common/game/main_game_file.h"
#include "ags/common/game/plugininfo.h"
#include "ags/common/game/roomstruct.h"
#include "ags/common/game/room_file.h"
#include "ags/common/game/room_version.h"

#include "ags/common/gui/guibutton.h"
#include "ags/common/gui/guidefines.h"
#include "ags/common/gui/guiinv.h"
#include "ags/common/gui/guilabel.h"
#include "ags/common/gui/guilistbox.h"
#include "ags/common/gui/guimain.h"
#include "ags/common/gui/guiobject.h"
#include "ags/common/gui/guislider.h"
#include "ags/common/gui/guitextbox.h"

#include "ags/common/util/alignedstream.h"
#include "ags/common/util/bbop.h"
#include "ags/common/util/bufferedstream.h"
#include "ags/common/util/compress.h"
#include "ags/common/util/datastream.h"
#include "ags/common/util/directory.h"
#include "ags/common/util/error.h"
#include "ags/common/util/file.h"
#include "ags/common/util/filestream.h"
#include "ags/common/util/geometry.h"
#include "ags/common/util/inifile.h"
//#include "ags/common/util/ini_util.h"
#include "ags/common/util/lzw.h"
#include "ags/common/util/math.h"
#include "ags/common/util/memory.h"
#include "ags/common/util/misc.h"
#include "ags/common/util/multifilelib.h"
#include "ags/common/util/path.h"
#include "ags/common/util/proxystream.h"
#include "ags/common/util/stdio_compat.h"
#include "ags/common/util/stream.h"
#include "ags/common/util/string.h"
#include "ags/common/util/string_compat.h"
#include "ags/common/util/string_types.h"
#include "ags/common/util/string_utils.h"
#include "ags/common/util/textreader.h"
#include "ags/common/util/textstreamreader.h"
#include "ags/common/util/textstreamwriter.h"
#include "ags/common/util/textwriter.h"
#include "ags/common/util/version.h"
#include "ags/common/util/wgt2allg.h"

namespace AGS {

AGSEngine::AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc) : Engine(syst),
		_gameDescription(gameDesc), _randomSource("AGS") {
	DebugMan.addDebugChannel(kDebugPath, "Path", "Pathfinding debug level");
	DebugMan.addDebugChannel(kDebugGraphics, "Graphics", "Graphics debug level");
}

AGSEngine::~AGSEngine() {
}

uint32 AGSEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::Error AGSEngine::run() {
	return Common::kNoError;
}

} // End of namespace AGS

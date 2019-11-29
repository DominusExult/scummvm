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

#ifndef AGS_COMMON_DYNOBJ_SCRIPTAUDIOCLIP_H
#define AGS_COMMON_DYNOBJ_SCRIPTAUDIOCLIP_H

#include "ags/common/util/string.h"

namespace AGS {
	
namespace AGSCommon { class Stream; }

enum AudioFileType {
    eAudioFileOGG = 1,
    eAudioFileMP3 = 2,
    eAudioFileWAV = 3,
    eAudioFileVOC = 4,
    eAudioFileMIDI = 5,
    eAudioFileMOD = 6
};

#define SCRIPTAUDIOCLIP_SCRIPTNAMELENGTH    30
#define SCRIPTAUDIOCLIP_FILENAMELENGTH      15

struct ScriptAudioClip {
    int id;
    AGSCommon::String scriptName;
	AGSCommon::String fileName;
    char bundlingType;
    char type;
    char fileType;
    char defaultRepeat;
    short defaultPriority;
    short defaultVolume;

    void ReadFromFile(AGSCommon::Stream *in);
};

} // End of namespace AGS

#endif

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

#ifndef AGS_COMMON_AC_VIEW_H
#define AGS_COMMON_AC_VIEW_H

#include "ags/std/vector.h"

namespace AGS {

namespace AGSCommon { class Stream; }

#define VFLG_FLIPSPRITE 1

struct ViewFrame {
    int   pic;
    short xoffs, yoffs;
    short speed;
    int   flags;
    int   sound;  // play sound when this frame comes round
    int   reserved_for_future[2];
    ViewFrame();

    void ReadFromFile(AGSCommon::Stream *in);
    void WriteToFile(AGSCommon::Stream *out);
};

#define LOOPFLAG_RUNNEXTLOOP 1

struct ViewLoopNew
{
    short numFrames;
    int   flags;
    ViewFrame *frames;

    ViewLoopNew();
    void Initialize(int frameCount);
    void Dispose();
    bool RunNextLoop();
    void WriteToFile_v321(AGSCommon::Stream *out);
    void ReadFromFile_v321(AGSCommon::Stream *in);
    void WriteFrames_Aligned(AGSCommon::Stream *out);
    void ReadFrames_Aligned(AGSCommon::Stream *in);
};

struct ViewStruct
{
    short numLoops;
    ViewLoopNew *loops;

    ViewStruct();
    void Initialize(int loopCount);
    void Dispose();
    void WriteToFile(AGSCommon::Stream *out);
    void ReadFromFile(AGSCommon::Stream *in);
};

struct ViewStruct272 {
    short     numloops;
    short     numframes[16];
    int       loopflags[16];
    ViewFrame frames[16][20];

    ViewStruct272();
    void ReadFromFile(AGSCommon::Stream *in);
};

void Convert272ViewsToNew(const std::vector<ViewStruct272> &oldv, ViewStruct *newv);

} // End of namespace AGS

#endif

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

#ifndef AGS_COMMON_CORE_PLATFORM_H
#define AGS_COMMON_CORE_PLATFORM_H

namespace AGS {

// platform definitions. For ScummVM we treat the platform as if it were Windows
#define AGS_PLATFORM_OS_WINDOWS (1)
#define AGS_PLATFORM_OS_LINUX   (0)
#define AGS_PLATFORM_OS_MACOS   (0)
#define AGS_PLATFORM_OS_ANDROID (0)
#define AGS_PLATFORM_OS_IOS     (0)
#define AGS_PLATFORM_OS_PSP     (0)

#define INT32_MIN_VAL -0x7fffffff
#define INT32_MAX_VAL 0x7fffffff
#define UINT32_MAX_VAL 0xffffffff

} // End of namespace AGS

#endif

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

#ifndef AGS_COMMON_UTIL_MISC_H
#define AGS_COMMON_UTIL_MISC_H

#include "ags/common/util/file.h" // TODO: extract filestream mode constants

namespace AGS {
	
namespace Shared { class Stream; }

// Case-insensitive file lookup functions. On case-insensitive systems
// (like MS Windows) they simply return given file, but on case-sensitive
// systems (like Linux) they search the directory for files with matching
// names in different character case.
// They are used as a system-independent way to open a file when its name
// case can be ignored.
Shared::Stream *ci_fopen(const char *file_name,
	Shared::FileOpenMode open_mode = Shared::kFile_Open,
	Shared::FileWorkMode work_mode = Shared::kFile_Read);

// TODO: return String object
char *ci_find_file(const char *dir_name, const char *file_name);

} // End of namespace AGS

#endif

//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
// Platform-independent Directory functions
//
//=============================================================================

#ifndef AGS_COMMON_UTIL_DIRECTORY_H
#define AGS_COMMON_UTIL_DIRECTORY_H

#include "ags/common/core/platform.h"
#include "ags/common/util/string.h"

namespace AGS {
namespace Shared {
namespace Directory {

// Creates new directory (if it does not exist)
bool   CreateDirectory(const String &path);
// Makes sure all directories in the path are created. Parent path is
// not touched, and function must fail if parent path is not accessible.
bool   CreateAllDirectories(const String &parent, const String &path);
// Sets current working directory, returns the resulting path
String SetCurrentDirectory(const String &path);
// Gets current working directory
String GetCurrentDirectory();

} // End of namespace Directory
} // End of namespace Shared
} // End of namespace AGS

#endif

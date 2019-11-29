#ifndef AGS_COMMON_CORE_DEFVERSION_H
#define AGS_COMMON_CORE_DEFVERSION_H

#define ACI_VERSION_STR      "3.5.0.19"
#if defined (RC_INVOKED) // for MSVC resource compiler
#define ACI_VERSION_MSRC_DEF  3,5,0,19
#endif

#ifdef NO_MP3_PLAYER
#define SPECIAL_VERSION "NMP"
#else
#define SPECIAL_VERSION ""
#endif

#define ACI_COPYRIGHT_YEARS "2011-2019"

#endif // AGS_COMMON_CORE_DEFVERSION_H

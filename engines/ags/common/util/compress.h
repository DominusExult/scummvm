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

#ifndef AGS_COMMON_UTIL_COMPRESS_H
#define AGS_COMMON_UTIL_COMPRESS_H

#include "ags/common/util/wgt2allg.h" // color (allegro RGB)

namespace AGS {

namespace Shared { class Stream; class Bitmap; }

void csavecompressed(Shared::Stream *out, const unsigned char * tobesaved, const color pala[256]);
// RLE compression
void cpackbitl(const uint8_t *line, int size, Shared::Stream *out);
void cpackbitl16(const uint16_t *line, int size, Shared::Stream *out);
void cpackbitl32(const uint32_t *line, int size, Shared::Stream *out);
// RLE decompression
int  cunpackbitl(uint8_t *line, int size, Shared::Stream *in);
int  cunpackbitl16(uint16_t *line, int size, Shared::Stream *in);
int  cunpackbitl32(uint32_t *line, int size, Shared::Stream *in);

//=============================================================================

void save_lzw(Shared::Stream *out, const Shared::Bitmap *bmpp, const color *pall);
void load_lzw(Shared::Stream *in, Shared::Bitmap **bmm, int dst_bpp, color *pall);
void savecompressed_allegro(Shared::Stream *out, const Shared::Bitmap *bmpp, const color *pall);
void loadcompressed_allegro(Shared::Stream *in, Shared::Bitmap **bimpp, color *pall);

} // End of namespace AGS

#endif

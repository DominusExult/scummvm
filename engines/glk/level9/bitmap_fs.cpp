/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "glk/level9/bitmap_fs.h"
#include "common/algorithm.h"
#include "common/memstream.h"

namespace Glk {
namespace Level9 {

BitmapType BitmapFileSystem::_bitmapType = NO_BITMAPS;

bool BitmapFileSystem::exists() {
	Common::String file = bitmap_noext_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = bitmap_noext_type(file);
		goto finish;
	}

	file = bitmap_pc_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = bitmap_pc_type(file);
		goto finish;
	}

	file = bitmap_c64_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = bitmap_c64_type(file);
		goto finish;
	}

	file = bitmap_bbc_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = BBC_BITMAPS;
		goto finish;
	}

	file = bitmap_cpc_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = CPC_BITMAPS;
		goto finish;
	}

	file = bitmap_st2_name(2);
	if (Common::File::exists(file)) {
		_bitmapType = ST2_BITMAPS;
		goto finish;
	}

	_bitmapType = NO_BITMAPS;
finish:	
	return _bitmapType != NO_BITMAPS;
}

void BitmapFileSystem::getPicInfo(int picNum, uint16 &width, uint16 &height, int &colorCount) {
	bool usage[256];
	Bitmap bitmap;
	if (!decodeBitmap(bitmap, picNum))
		error("Invalid number %d specified", picNum);

	width = bitmap.w;
	height = bitmap.h;
	colorCount = 0;

	// Traverse the image
	Common::fill(&usage[0], &usage[256], false);

	for (int y = 0; y < bitmap.h; ++y) {
		const byte *lineP = (const byte *)bitmap.getBasePtr(0, y);
		for (int x = 0; x < bitmap.w; ++x, ++lineP) {
			if (!usage[*lineP]) {
				usage[*lineP] = true;
				++colorCount;
			}
		}
	}
}


byte *BitmapFileSystem::bitmap_load(const Common::String &file, size_t *size) {
	byte *data = nullptr;

	Common::File f;
	if (f.open(file)) {
		*size = f.size();
		data = (byte *)malloc(*size);
		f.read(data, *size);

		f.close();
	}

	return data;
}

Colour BitmapFileSystem::bitmap_pcst_colour(int big, int small) {
	Colour col;
	uint32 r = big & 0xF;
	uint32 g = (small >> 4) & 0xF;
	uint32 b = small & 0xF;

	r *= 0x49;
	r >>= 1;
	g *= 0x49;
	g >>= 1;
	b *= 0x49;
	b >>= 1;

	col.red = (byte)(r & 0xFF);
	col.green = (byte)(g & 0xFF);
	col.blue = (byte)(b & 0xFF);
	return col;
}

Common::String BitmapFileSystem::bitmap_noext_name(int num) {
	if (num == 0) {
		if (Common::File::exists("title"))
			return "title";

		num = 30;
	}

	return Common::String::format("%d", num);
}

BitmapType BitmapFileSystem::bitmap_noext_type(const Common::String &file) {
	Common::File f;
	if (f.open(file)) {
		byte data[72];
		int x, y;

		if (f.read(data, sizeof(data)) != sizeof(data) && !f.eos())
			return NO_BITMAPS;
		f.close();

		x = data[67] + data[66] * 256;
		y = data[71] + data[70] * 256;

		if ((x == 0x0140) && (y == 0x0088))
			return AMIGA_BITMAPS;
		if ((x == 0x0140) && (y == 0x0087))
			return AMIGA_BITMAPS;
		if ((x == 0x00E0) && (y == 0x0075))
			return AMIGA_BITMAPS;
		if ((x == 0x00E4) && (y == 0x0075))
			return AMIGA_BITMAPS;
		if ((x == 0x00E0) && (y == 0x0076))
			return AMIGA_BITMAPS;
		if ((x == 0x00DB) && (y == 0x0076))
			return AMIGA_BITMAPS;

		x = data[3] + data[2] * 256;
		y = data[7] + data[6] * 256;

		if ((x == 0x0200) && (y == 0x00D8))
			return MAC_BITMAPS;
		if ((x == 0x0168) && (y == 0x00BA))
			return MAC_BITMAPS;
		if ((x == 0x0168) && (y == 0x00BC))
			return MAC_BITMAPS;
		if ((x == 0x0200) && (y == 0x00DA))
			return MAC_BITMAPS;
		if ((x == 0x0168) && (y == 0x00DA))
			return MAC_BITMAPS;

		x = data[35] + data[34] * 256;
		y = data[39] + data[38] * 256;

		if ((x == 0x0050) && (y == 0x0087))
			return ST1_BITMAPS;
		if ((x == 0x0038) && (y == 0x0074))
			return ST1_BITMAPS;
	}

	return NO_BITMAPS;
}

Common::String BitmapFileSystem::bitmap_pc_name(int num) {
	// title picture is #30
	if (num == 0)
		num = 30;
	
	return Common::String::format("%d.pic", num);
}

Colour BitmapFileSystem::bitmap_pc1_colour(int i) {
	Colour col;
	col.red = (((i & 4) >> 1) | ((i & 0x20) >> 5)) * 0x55;
	col.green = ((i & 2) | ((i & 0x10) >> 4)) * 0x55;
	col.blue = (((i & 1) << 1) | ((i & 8) >> 3)) * 0x55;
	return col;
}

bool BitmapFileSystem::bitmap_pc1_decode(Bitmap &bitmap, const Common::String &file, int x, int y) {
	byte *data = nullptr;
	int i, xi, yi, max_x, max_y;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == nullptr)
		return false;

	max_x = data[2] + data[3] * 256;
	max_y = data[4] + data[5] * 256;
	if (max_x > MAX_BITMAP_WIDTH || max_y > MAX_BITMAP_HEIGHT) {
		free(data);
		return false;
	}

	if ((x == 0) && (y == 0))
		bitmap.create(max_x, max_y);
	if (bitmap.empty()) {
		free(data);
		return false;
	}

	if (x + max_x > bitmap.w)
		max_x = bitmap.w - x;
	if (y + max_y > bitmap.h)
		max_y = bitmap.h - y;

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(x, y + yi);
		for (xi = 0; xi < max_x; xi++, lineP++) {
			*lineP = (data[23 + ((yi * max_x) / 2) + (xi / 2)] >> ((1 - (xi & 1)) * 4)) & 0x0f;
		}
	}

	bitmap._palette.resize(16 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 16; ++i, pp += 3) {
		Colour c = bitmap_pc1_colour(data[6 + i]);
		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	free(data);
	return true;
}

bool BitmapFileSystem::bitmap_pc2_decode(Bitmap &bitmap, const Common::String &file, int x, int y) {
	byte *data = nullptr;
	int i, xi, yi, max_x, max_y;

	byte theNewPixel, theNewPixelIndex;
	byte theBufferBitCounter, theNewPixelIndexSelector, theBufferBitStripCount;
	uint16 theBitStreamBuffer, theImageDataIndex;
	byte *theImageFileData;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == nullptr)
		return false;

	max_x = data[37] + data[36] * 256;
	max_y = data[39] + data[38] * 256;
	if (max_x > MAX_BITMAP_WIDTH || max_y > MAX_BITMAP_HEIGHT) {
		free(data);
		return false;
	}

	if ((x == 0) && (y == 0))
		bitmap.create(max_x, max_y);
	if (bitmap.empty()) {
		free(data);
		return false;
	}

	if (x + max_x > bitmap.w)
		max_x = bitmap.w - x;
	if (y + max_y > bitmap.h)
		max_y = bitmap.h - y;

	// prime the new pixel variable with the seed byte
	theNewPixel = data[40];
	// initialise the index to the image data 
	theImageDataIndex = 0;
	// prime the bit stream buffer
	theImageFileData = data + 570;
	theBitStreamBuffer = theImageFileData[theImageDataIndex++];
	theBitStreamBuffer = theBitStreamBuffer +
		(0x100 * theImageFileData[theImageDataIndex++]);
	// initialise the bit stream buffer bit counter
	theBufferBitCounter = 8;

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));

	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(x, y + yi);
		
		for (xi = 0; xi < max_x; xi++, lineP++) {
			theNewPixelIndexSelector = (theBitStreamBuffer & 0x00FF);
			if (theNewPixelIndexSelector != 0xFF) {
				// get index for new pixel and bit strip count
				theNewPixelIndex = (data + 314)[theNewPixelIndexSelector];
				// get the bit strip count
				theBufferBitStripCount = (data + 298)[theNewPixelIndex];
				// strip theBufferBitStripCount bits from theBitStreamBuffer
				while (theBufferBitStripCount > 0) {
					theBitStreamBuffer = theBitStreamBuffer >> 1;
					theBufferBitStripCount--;
					theBufferBitCounter--;
					if (theBufferBitCounter == 0) {
						// need to refill the theBitStreamBuffer high byte
						theBitStreamBuffer = theBitStreamBuffer +
							(0x100 * theImageFileData[theImageDataIndex++]);
						// re-initialise the bit stream buffer bit counter
						theBufferBitCounter = 8;
					}
				}
			} else {
				// strip the 8 bits holding 0xFF from theBitStreamBuffer
				theBufferBitStripCount = 8;
				while (theBufferBitStripCount > 0) {
					theBitStreamBuffer = theBitStreamBuffer >> 1;
					theBufferBitStripCount--;
					theBufferBitCounter--;
					if (theBufferBitCounter == 0) {
						// need to refill the theBitStreamBuffer high byte
						theBitStreamBuffer = theBitStreamBuffer +
							(0x100 * theImageFileData[theImageDataIndex++]);
						// re-initialise the bit stream buffer bit counter
						theBufferBitCounter = 8;
					}
				}
				
				// get the literal pixel index value from the bit stream
				theNewPixelIndex = (0x000F & theBitStreamBuffer);
				theBufferBitStripCount = 4;
				// strip 4 bits from theBitStreamBuffer
				while (theBufferBitStripCount > 0) {
					theBitStreamBuffer = theBitStreamBuffer >> 1;
					theBufferBitStripCount--;
					theBufferBitCounter--;
					if (theBufferBitCounter == 0) {
						// need to refill the theBitStreamBuffer high byte
						theBitStreamBuffer = theBitStreamBuffer +
							(0x100 * theImageFileData[theImageDataIndex++]);
						// re-initialise the bit stream buffer bit counter
						theBufferBitCounter = 8;
					}
				}
			}

			// shift the previous pixel into the high four bits of theNewPixel
			theNewPixel = (0xF0 & (theNewPixel << 4));
			// add the index to the new pixel to theNewPixel
			theNewPixel = theNewPixel + theNewPixelIndex;
			// extract the nex pixel from the table
			theNewPixel = (data + 42)[theNewPixel];
			// store new pixel in the bitmap
			*lineP = theNewPixel;
		}
	}

	bitmap._palette.resize(16 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 16; ++i, pp += 3) {
		Colour c = bitmap_pcst_colour(data[4 + (i * 2)], data[5 + (i * 2)]);
		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	free(data);
	return true;
}

BitmapType BitmapFileSystem::bitmap_pc_type(const Common::String &file) {
	BitmapType type = PC2_BITMAPS;

	Common::File f;
	if (f.open(file)) {
		byte data[6];
		int x, y;

		if (f.read(data, sizeof(data)) != sizeof(data) && !f.eos())
			return NO_BITMAPS;
		f.close();

		x = data[2] + data[3] * 256;
		y = data[4] + data[5] * 256;

		if ((x == 0x0140) && (y == 0x0087))
			type = PC1_BITMAPS;
		if ((x == 0x00E0) && (y == 0x0074))
			type = PC1_BITMAPS;
		if ((x == 0x0140) && (y == 0x0087))
			type = PC1_BITMAPS;
		if ((x == 0x00E1) && (y == 0x0076))
			type = PC1_BITMAPS;
	}

	return type;
}

uint32 BitmapFileSystem::bitmap_st1_decode_pixels(byte *pic, byte *data, uint32 count, uint32 pixels) {
	uint32 bitplane_length = count / 4; // length of each bitplane
	byte *bitplane0 = data; // address of bit0 bitplane
	byte *bitplane1 = data + (bitplane_length); // address of bit1 bitplane
	byte *bitplane2 = data + (bitplane_length * 2); // address of bit2 bitplane
	byte *bitplane3 = data + (bitplane_length * 3); // address of bit3 bitplane
	uint32 bitplane_index, pixel_index = 0; // index variables

	for (bitplane_index = 0; bitplane_index < bitplane_length; bitplane_index++) {
		// build the eight pixels from the current bitplane bytes, high bit to low

		// bit7 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] >> 4) & 0x08)
			+ ((bitplane2[bitplane_index] >> 5) & 0x04)
			+ ((bitplane1[bitplane_index] >> 6) & 0x02)
			+ ((bitplane0[bitplane_index] >> 7) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit6 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] >> 3) & 0x08)
			+ ((bitplane2[bitplane_index] >> 4) & 0x04)
			+ ((bitplane1[bitplane_index] >> 5) & 0x02)
			+ ((bitplane0[bitplane_index] >> 6) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit5 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] >> 2) & 0x08)
			+ ((bitplane2[bitplane_index] >> 3) & 0x04)
			+ ((bitplane1[bitplane_index] >> 4) & 0x02)
			+ ((bitplane0[bitplane_index] >> 5) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit4 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] >> 1) & 0x08)
			+ ((bitplane2[bitplane_index] >> 2) & 0x04)
			+ ((bitplane1[bitplane_index] >> 3) & 0x02)
			+ ((bitplane0[bitplane_index] >> 4) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit3 byte
		pic[pixel_index] = ((bitplane3[bitplane_index]) & 0x08)
			+ ((bitplane2[bitplane_index] >> 1) & 0x04)
			+ ((bitplane1[bitplane_index] >> 2) & 0x02)
			+ ((bitplane0[bitplane_index] >> 3) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit2 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] << 1) & 0x08)
			+ ((bitplane2[bitplane_index]) & 0x04)
			+ ((bitplane1[bitplane_index] >> 1) & 0x02)
			+ ((bitplane0[bitplane_index] >> 2) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit1 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] << 2) & 0x08)
			+ ((bitplane2[bitplane_index] << 1) & 0x04)
			+ ((bitplane1[bitplane_index]) & 0x02)
			+ ((bitplane0[bitplane_index] >> 1) & 0x01);
		if (pixels == ++pixel_index)
			break;

		// bit0 byte
		pic[pixel_index] = ((bitplane3[bitplane_index] << 3) & 0x08)
			+ ((bitplane2[bitplane_index] << 2) & 0x04)
			+ ((bitplane1[bitplane_index] << 1) & 0x02)
			+ ((bitplane0[bitplane_index]) & 0x01);
		if (pixels == ++pixel_index)
			break;
	}

	return pixel_index;
}

bool BitmapFileSystem::bitmap_st1_decode(Bitmap &bitmap, const Common::String &file, int x, int y) {
	byte *data = NULL;
	int i, xi, yi, max_x, max_y, last_block;
	int bitplanes_row, bitmaps_row, pixel_count, get_pixels;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == NULL)
		return false;

	bitplanes_row = data[35] + data[34] * 256;
	bitmaps_row = bitplanes_row / 4;
	max_x = bitplanes_row * 4;
	max_y = data[39] + data[38] * 256;
	last_block = data[43] + data[42] * 256;

	// Check if sub-image with rows shorter than max_x
	if (last_block != 0xFFFF) {
		// use last_block to adjust max_x
		i = 0;
		while ((0x0001 & last_block) == 0) { // test for ls bit set
			last_block >>= 1; // if not, shift right one bit
			i++;
		}
		max_x = max_x - i;
	}

	if (max_x > MAX_BITMAP_WIDTH || max_y > MAX_BITMAP_HEIGHT) {
		free(data);
		return false;
	}

	if ((x == 0) && (y == 0))
		bitmap.create(max_x, max_y);
	if (bitmap.empty()) {
		free(data);
		return false;
	}

	if (x + max_x > bitmap.w)
		max_x = bitmap.w - x;
	if (y + max_y > bitmap.h)
		max_y = bitmap.h - y;

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(x, y + yi);
		pixel_count = 0;
		for (xi = 0; xi < bitmaps_row; xi++) {
			if ((max_x - pixel_count) < 16)
				get_pixels = max_x - pixel_count;
			else
				get_pixels = 16;

			pixel_count += bitmap_st1_decode_pixels(lineP + xi * 16,
				data + 44 + (yi * bitplanes_row * 2) + (xi * 8), 8, get_pixels);
		}
	}

	bitmap._palette.resize(16 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 16; ++i, pp += 3) {
		Colour c = bitmap_pcst_colour(data[(i * 2)], data[1 + (i * 2)]);
		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	free(data);
	return true;
}

Common::String BitmapFileSystem::bitmap_st2_name(int num) {
	// title picture is #30
	if (num == 0)
		num = 30;

	return Common::String::format("%d.squ", num);
}

Colour BitmapFileSystem::bitmap_amiga_colour(int i1, int i2) {
	Colour col;
	col.red = bitmap_amiga_intensity(i1 & 0xf);
	col.green = bitmap_amiga_intensity(i2 >> 4);
	col.blue = bitmap_amiga_intensity(i2 & 0xf);
	return col;
}

bool BitmapFileSystem::bitmap_amiga_decode(Bitmap &bitmap, const Common::String &file, int x, int y) {
	byte *data = NULL;
	int i, xi, yi, max_x, max_y, p, b;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == NULL)
		return false;

	max_x = (((((data[64] << 8) | data[65]) << 8) | data[66]) << 8) | data[67];
	max_y = (((((data[68] << 8) | data[69]) << 8) | data[70]) << 8) | data[71];
	if (max_x > MAX_BITMAP_WIDTH || max_y > MAX_BITMAP_HEIGHT) {
		free(data);
		return false;
	}

	if ((x == 0) && (y == 0))
		bitmap.create(max_x, max_y);
	if (bitmap.empty()) {
		free(data);
		return false;
	}

	if (x + max_x > bitmap.w)
		max_x = bitmap.w - x;
	if (y + max_y > bitmap.h)
		max_y = bitmap.h - y;

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(x, y + yi);

		for (xi = 0; xi < max_x; xi++, lineP++) {
			p = 0;
			for (b = 0; b < 5; b++)
				p |= ((data[72 + (max_x / 8) * (max_y * b + yi) + xi / 8] >> (7 - (xi % 8))) & 1) << b;
			*lineP = p;
		}
	}

	bitmap._palette.resize(32 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 32; ++i, pp += 3) {
		Colour c = bitmap_amiga_colour(data[i * 2], data[i * 2 + 1]);
		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	free(data);
	return true;
}

bool BitmapFileSystem::bitmap_mac_decode(Bitmap &bitmap, const Common::String &file, int x, int y) {
	byte *data = NULL;
	int xi, yi, max_x, max_y;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == NULL)
		return false;

	max_x = data[3] + data[2] * 256;
	max_y = data[7] + data[6] * 256;
	if (max_x > MAX_BITMAP_WIDTH || max_y > MAX_BITMAP_HEIGHT) {
		free(data);
		return false;
	}

	if (x > 0)  // Mac bug, apparently
		x = 78;

	if ((x == 0) && (y == 0))
		bitmap.create(max_x, max_y);
	if (bitmap.empty()) {
		free(data);
		return false;
	}

	if (x + max_x > bitmap.w)
		max_x = bitmap.w - x;
	if (y + max_y > bitmap.h)
		max_y = bitmap.h - y;

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(x, y + yi);

		for (xi = 0; xi < max_x; xi++, ++lineP) {
			*lineP = (data[10 + (max_x / 8) * yi + xi / 8] >> (7 - (xi % 8))) & 1;
		}
	}

	bitmap._palette.resize(2 * 3);
	bitmap._palette[0] = 0;
	bitmap._palette[1] = 0;
	bitmap._palette[2] = 0;
	bitmap._palette[3] = 0xff;
	bitmap._palette[4] = 0xff;
	bitmap._palette[5] = 0xff;

	free(data);
	return true;
}

// Commodore 64 palette from Vice
const Colour bitmap_c64_colours[] = {
	{0x00, 0x00, 0x00 },
	{0xff, 0xff, 0xff },
	{0x89, 0x40, 0x36 },
	{0x7a, 0xbf, 0xc7 },
	{0x8a, 0x46, 0xae },
	{0x68, 0xa9, 0x41 },
	{0x3e, 0x31, 0xa2 },
	{0xd0, 0xdc, 0x71 },
	{0x90, 0x5f, 0x25 },
	{0x5c, 0x47, 0x00 },
	{0xbb, 0x77, 0x6d },
	{0x55, 0x55, 0x55 },
	{0x80, 0x80, 0x80 },
	{0xac, 0xea, 0x88 },
	{0x7c, 0x70, 0xda },
	{0xab, 0xab, 0xab }
};

const Colour bitmap_bbc_colours[] = {
	{0x00, 0x00, 0x00 },
	{0xff, 0x00, 0x00 },
	{0x00, 0xff, 0x00 },
	{0xff, 0xff, 0x00 },
	{0x00, 0x00, 0xff },
	{0xff, 0x00, 0xff },
	{0x00, 0xff, 0xff },
	{0xff, 0xff, 0xff }
};

Common::String BitmapFileSystem::bitmap_c64_name(int num) {
	if (num == 0)
		return "stitle mpic";
	else
		return Common::String::format("pic%d", num);
}

Common::String BitmapFileSystem::bitmap_bbc_name(int num) {
	if (num == 0) {
		if (Common::File::exists("P.Title"))
			return "P.Title";

		return "title";
	} else {
		Common::String file = Common::String::format("P.Pic%d", num);

		if (Common::File::exists(file))
			return file;

		return Common::String::format("pic%d", num);
	}
}

Common::String BitmapFileSystem::bitmap_cpc_name(int num) {
	if (num == 0)
		return "title.pic";
	else if (num == 1)
		return "1.pic";
	else
		return "allpics.pic";
}

BitmapType BitmapFileSystem::bitmap_c64_type(const Common::String &file) {
	BitmapType type = C64_BITMAPS;

	Common::File f;
	if (f.open(file)) {
		uint32 size = f.size();
		f.close();

		if (size == 10048)
			type = BBC_BITMAPS;
		if (size == 6494)
			type = BBC_BITMAPS;
	}

	return type;
}

bool BitmapFileSystem::bitmap_c64_decode(Bitmap &bitmap, const Common::String &file, BitmapType type, int num) {
	byte *data = NULL;
	int i = 0, xi, yi, max_x = 0, max_y = 0, cx, cy, px, py, p;
	int off = 0, off_scr = 0, off_col = 0, off_bg = 0, col_comp = 0;

	size_t size;
	data = bitmap_load(file, &size);
	if (data == NULL)
		return false;

	if (type == C64_BITMAPS) {
		if (size == 10018) { // C64 title picture
			max_x = 320;
			max_y = 200;
			off = 2;
			off_scr = 8002;
			off_bg = 9003;
			off_col = 9018;
			col_comp = 0;
		} else if (size == 6464) { // C64 picture
			max_x = 320;
			max_y = 136;
			off = 2;
			off_scr = 5442;
			off_col = 6122;
			off_bg = 6463;
			col_comp = 1;
		} else
			return false;
	} else if (type == BBC_BITMAPS) {
		if (size == 10058) { // BBC title picture
			max_x = 320;
			max_y = 200;
			off = 10;
			off_scr = 8010;
			off_bg = 9011;
			off_col = 9026;
			col_comp = 0;
		} else if (size == 10048) { // BBC title picture
			max_x = 320;
			max_y = 200;
			off = 0;
			off_scr = 8000;
			off_bg = 9001;
			off_col = 9016;
			col_comp = 0;
		} else if (size == 6504) { // BBC picture
			max_x = 320;
			max_y = 136;
			off = 10;
			off_scr = 5450;
			off_col = 6130;
			off_bg = 6471;
			col_comp = 1;
		} else if (size == 6494) { // BBC picture
			max_x = 320;
			max_y = 136;
			off = 0;
			off_scr = 5440;
			off_col = 6120;
			off_bg = 6461;
			col_comp = 1;
		} else
			return false;
	} else if (type == CPC_BITMAPS) {
		if (num == 0) { // CPC/+3 title picture
			max_x = 320;
			max_y = 200;
			off = 128;
			off_scr = 8128;
			off_bg = 9128;
			off_col = 9144;
			col_comp = 0;
		} else if (num == 1) { // First CPC/+3 picture
			max_x = 320;
			max_y = 136;
			off = 128;
			off_scr = 5568;
			off_col = 6248;
			off_bg = 6588;
			col_comp = 1;
		} else if (num >= 2 && num <= 29) { // Subsequent CPC/+3 pictures
			max_x = 320;
			max_y = 136;
			off = ((num - 2) * 6462);
			off_scr = 5440 + ((num - 2) * 6462);
			off_col = 6120 + ((num - 2) * 6462);
			off_bg = 6460 + ((num - 2) * 6462);
			col_comp = 1;
		} else
			return false;
	}

	bitmap.create(max_x, max_y);

	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	for (yi = 0; yi < max_y; yi++) {
		byte *lineP = (byte *)s.getBasePtr(0, yi);

		for (xi = 0; xi < max_x / 2; xi++) {
			cx = xi / 4;
			px = xi % 4;
			cy = yi / 8;
			py = yi % 8;

			p = data[off + (cy * 40 + cx) * 8 + py];
			p = (p >> ((3 - px) * 2)) & 3;

			switch (p) {
			case 0:
				i = data[off_bg] & 0x0f;
				break;
			case 1:
				i = data[off_scr + cy * 40 + cx] >> 4;
				break;
			case 2:
				i = data[off_scr + cy * 40 + cx] & 0x0f;
				break;
			case 3:
				if (col_comp)
					i = (data[off_col + (cy * 40 + cx) / 2] >> ((1 - (cx % 2)) * 4)) & 0x0f;
				else
					i = data[off_col + (cy * 40 + cx)] & 0x0f;
				break;
			}

			lineP[xi * 2] = i;
			lineP[xi * 2 + 1] = i;
		}
	}

	bitmap._palette.resize(16 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 32; ++i, pp += 3) {
		Colour c = bitmap_c64_colours[i];

		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	free(data);
	return true;
}

bool BitmapFileSystem::bitmap_bbc_decode(Bitmap &bitmap, const Common::String &file, BitmapType type, int num) {
	unsigned char   patRowData[32];
	unsigned char patArray[16][2][2];
	int i, j, k, isOddColumn, isOddRow;
	byte pixel;

	if (bitmap_c64_decode(bitmap, file, type, num) == false)
		return false;

	Common::File f;
	if (!f.open(file))
		return false;

	// Seek to the offset of the pixPat data and read in the data
	f.seek(f.size() - 32, SEEK_SET);
	if (f.read(patRowData, 32) != 32 && !f.eos())
		return false;
	f.close();

	// Extract the patterns
	i = 0;
	for (k = 0; k < 2; k++) {
		for (j = 0; j < 16; j++) {
			// Extract the even col pixel for this pattern row
			patArray[j][k][0] =
				((patRowData[i] >> 4) & 0x8) + ((patRowData[i] >> 3) & 0x4) +
				((patRowData[i] >> 2) & 0x2) + ((patRowData[i] >> 1) & 0x1);
			// Extract the odd col pixel for this pattern row
			patArray[j][k][1] =
				((patRowData[i] >> 3) & 0x8) + ((patRowData[i] >> 2) & 0x4) +
				((patRowData[i] >> 1) & 0x2) + (patRowData[i] & 0x1);
			i++;
		}
	}

	// Convert the image. Each BBC pixel is represented by two pixels here
	Graphics::Surface s = bitmap.getSubArea(Common::Rect(0, 0, bitmap.w, bitmap.h));
	byte *pic = (byte *)s.getPixels();
	i = 0;
	isOddRow = 0;
	for (j = 0; j < bitmap.h; j++) {
		isOddColumn = 0;
		for (k = 0; k < bitmap.w / 2; k++) {
			pixel = pic[i];
			pic[i] = patArray[pixel][isOddColumn][isOddRow];
			pic[i + 1] = patArray[pixel][isOddColumn][isOddRow];
			isOddColumn ^= 1;
			i += 2;
		}
		isOddRow ^= 1;
	}

	bitmap._palette.resize(8 * 3);
	byte *pp = &bitmap._palette[0];
	for (i = 0; i < 8; ++i, pp += 3) {
		Colour c = bitmap_bbc_colours[i];

		pp[0] = c.red;
		pp[1] = c.green;
		pp[2] = c.blue;
	}

	return true;
}


/*--------------------------------------------------------------------------*/

BitmapFileSystem::BitmapFileSystem() : Common::Archive() {
	assert(_bitmapType != NO_BITMAPS);
}

bool BitmapFileSystem::hasFile(const Common::String &name) const {
	int picNum = getPictureNumber(name);
	return picNum != -1;
}

int BitmapFileSystem::getPictureNumber(const Common::String &filename) const {
	if (!filename.hasPrefixIgnoreCase("pic") || !filename.hasSuffixIgnoreCase(".raw"))
		return -1;

	Common::String numStr(filename.c_str() + 3, filename.c_str() + filename.size() - 4);
	return (numStr.empty() || !Common::isDigit(numStr[0])) ? -1 : atoi(numStr.c_str());
}

int BitmapFileSystem::listMembers(Common::ArchiveMemberList &list) const {
	// Listing isn't supported
	return 0;
}

const Common::ArchiveMemberPtr BitmapFileSystem::getMember(const Common::String &name) const {
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *BitmapFileSystem::createReadStreamForMember(const Common::String &name) const {
	// Get the picture number
	int picNum = getPictureNumber(name);
	if (picNum == -1)
		return nullptr;

	// Decode the picture
	Bitmap bitmap;
	if (!decodeBitmap(bitmap, picNum))
		return nullptr;

	// Create a memory stream for the raw picture data
	Common::MemoryWriteStreamDynamic mem(DisposeAfterUse::NO);

	// Add the dimensions
	mem.writeUint16LE(bitmap.w);
	mem.writeUint16LE(bitmap.h);

	// Add the palette
	mem.writeUint16LE(bitmap._palette.size() / 3);
	mem.write(&bitmap._palette[0], bitmap._palette.size());

	// No transparency
	mem.writeByte(0xff);

	// Write the pixels
	mem.write((const byte *)bitmap.getPixels(), bitmap.w * bitmap.h);

	// Pass out a read stream
	return new Common::MemoryReadStream(mem.getData(), mem.size(), DisposeAfterUse::YES);
}

bool BitmapFileSystem::decodeBitmap(Bitmap &bitmap, int num) {
	const int x = 0, y = 0;

	switch (_bitmapType) {
	case PC1_BITMAPS:
		if (bitmap_pc1_decode(bitmap, bitmap_pc_name(num), x, y))
			return true;
		break;

	case PC2_BITMAPS:
		if (bitmap_pc2_decode(bitmap, bitmap_pc_name(num), x, y))
			return true;
		break;

	case AMIGA_BITMAPS:
		if (bitmap_amiga_decode(bitmap, bitmap_noext_name(num), x, y))
			return true;
		break;

	case C64_BITMAPS:
		if (bitmap_c64_decode(bitmap, bitmap_c64_name(num), _bitmapType, num))
			return true;
		break;

	case BBC_BITMAPS:
		if (bitmap_bbc_decode(bitmap, bitmap_bbc_name(num), _bitmapType, num))
			return true;
		break;

	case CPC_BITMAPS:
		// Nearly identical to C64
		if (bitmap_c64_decode(bitmap, bitmap_cpc_name(num), _bitmapType, num))
			return true;
		break;

	case MAC_BITMAPS:
		if (bitmap_mac_decode(bitmap, bitmap_noext_name(num), x, y))
			return true;
		break;

	case ST1_BITMAPS:
		if (bitmap_st1_decode(bitmap, bitmap_noext_name(num), x, y))
			return true;
		break;

	case ST2_BITMAPS:
		if (bitmap_pc2_decode(bitmap, bitmap_st2_name(num), x, y))
			return true;
		break;

	default:
		break;
	}

	return false;
}

} // End of namespace Frotz
} // End of namespace Glk

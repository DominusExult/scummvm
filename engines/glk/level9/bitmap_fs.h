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

#ifndef GLK_LEVEL9_BITMAP_FS
#define GLK_LEVEL9_BITMAP_FS

#include "common/archive.h"
#include "common/array.h"
#include "common/file.h"
#include "graphics/managed_surface.h"

namespace Glk {
namespace Level9 {

enum BitmapType {
	NO_BITMAPS,
	AMIGA_BITMAPS,
	PC1_BITMAPS,
	PC2_BITMAPS,
	C64_BITMAPS,
	BBC_BITMAPS,
	CPC_BITMAPS,
	MAC_BITMAPS,
	ST1_BITMAPS,
	ST2_BITMAPS
};

struct Colour {
	byte red, green, blue;
};

#define MAX_BITMAP_WIDTH 512
#define MAX_BITMAP_HEIGHT 218

class Bitmap : public Graphics::ManagedSurface {
public:
	Common::Array<byte> _palette;
public:
	Bitmap() : Graphics::ManagedSurface() {}
	Bitmap(Graphics::ManagedSurface &surf) : Graphics::ManagedSurface(surf) {}
	Bitmap(int width, int height) : Graphics::ManagedSurface(width, height) {}
	Bitmap(int width, int height, const Graphics::PixelFormat &pixelFormat) :
		Graphics::ManagedSurface(width, height, pixelFormat) {}
	Bitmap(Graphics::ManagedSurface &surf, const Common::Rect &bounds) :
		Graphics::ManagedSurface(surf, bounds) {}
	virtual ~Bitmap() {}
};

/**
 * Level 9 graphics virtual filesystem
 */
class BitmapFileSystem : public Common::Archive {
private:
	static Common::String bitmap_noext_name(int num);
	static byte *bitmap_load(const Common::String &file, size_t *size);
	static BitmapType bitmap_noext_type(const Common::String &file);

	/**
	 * A PC or ST palette colour is a sixteen bit value in which the low three nybbles
	 * hold the rgb colour values. The lowest nybble holds the blue value, the
	 * second nybble the blue value and the third nybble the red value. (The high
	 * nybble is ignored). Within each nybble, only the low three bits are used
	 * IE the value can only be 0-7 not the full possible 0-15 and so the MSbit in
	 * each nybble is always 0.
	*/
	static Colour bitmap_pcst_colour(int big, int small);

	/**
	 * @defgroup PC Bitmaps
	 *
	 * On the PC different graphics file formats were used for the early V4
	 * games (Knight Orc, Gnome Ranger) and the later V4 games (Lancelot,
	 * Ingrid's Back, Time & Magik and Scapeghost).
	 *
	 * The ST and the PC both use the same image file format for the later
	 * V4 games (Lancelot, Ingrid's Back, Time & Magik and Scapeghost.)
	 * Mathamatical operations
	 */
	 //@{

	static Common::String bitmap_pc_name(int num);

	/**
	 * The EGA standard for the IBM PCs and compatibles defines 64 colors, any
	 * 16 of which can be mapped to the usable palette at any given time. If
	 * you display these 64 colors in numerical order, 16 at a time, you get a
	 * hodgepodge of colors in no logical order. The 64 EGA color numbers are
	 * assigned in a way that the numbers can easily be converted to a relative
	 * intensity of each of the three phosphor colors R,G,B. If the number is
	 * converted to six bit binary, the most significant three bits represent
	 * the 25% level of R,G,B in that order and the least significant three
	 * bits represent the 75% level of R,G,B in that order. Take EGA color 53
	 * for example. In binary, 53 is 110101. Since both R bits are on, R = 1.0.
	 * Of the G bits only the 25% bit is on so G = 0.25. Of the B bits only the
	 * 75% bit is on so B = 0.75.
	 */
	static Colour bitmap_pc1_colour(int i);

	/**
	 * The PC (v1) image file has the following format. It consists of a 22
	 * byte header organised like this:
	 * 
	 * Byte 0:     probably a file type flag
	 * Byte 1:     the MSB of the file's length as a word
	 * Bytes 2-3:  little-endian word with picture width in pixels
	 * Bytes 4-5:  little-endian word with picture height in pixel rows
	 * Bytes 6-21: the image colour table. One EGA colour in each byte
	 * 
	 * The image data is extremely simple. The entire block is packed array
	 * of 4-bit pixels - IE each byte holds two pixels - the first in the high
	 * nybble, the second in the low. The pixel value is an index into the
	 * image colour table. The pixels are organised with the top left first and
	 * bottom left last, each row in turn.
	*/
	static bool bitmap_pc1_decode(Bitmap &bitmap, const Common::String &file, int x, int y);

	/*
	 * The PC (v2) image file has the following format. It consists of a 44
	 * byte header followed by the image data.
	 * 
	 * The header has the following format:
	 * Bytes 0-1: "datalen":   length of file -1 as a big-endian word*
	 * Bytes 2-3: "flagbyte1 & flagbyte2": unknown, possibly type identifiers.
	 * 	Usually 0xFF or 0xFE followed by 0x84, 0x72, 0xFF, 0xFE or
	 * 	some other (of a fairly small range of possibles) byte.
	 * Bytes 4-35: "colour_index[]": sixteen entry palette. Basically an ST
	 * 	palette (even if in a PC image file. Each entry is a sixteen
	 * 	bit value in which the low three nybbles hold the rgb colour
	 * 	values. The lowest nybble holds the blue value, the second
	 * 	nybble the blue value and the third nybble the red value. (The
	 * 	high nybble is ignored). Within each nybble, only the low
	 * 	three bits are used IE the value can only be 0-7 not the full
	 * 	possible 0-15 and so the MSbit in each nybble is always 0.**,
	 * Bytes 36-37: "width": image width in pixels as a big-endian word
	 * Bytes 38-39: "numrows": image height in pixel rows as a big-endian word
	 * Byte 40: "seedByte": seed byte to start picture decoding.
	 * Byte 41: "padByte": unknown. Possibly padding to word align the next
	 * 	element?
	 * Bytes 42-297: "pixelTable": an array of 0x100 bytes used as a lookup table
	 * 	for pixel values
	 * Bytes 298-313: "bitStripTable": an array of 0x10 bytes used as a lookup table
	 * 	for the number of bytes to strip from the bit stream for the pixel being
	 * 	decoded
	 * Bytes 314-569:  "indexByteTable": an array of 0x100 bytes used as a lookup
	 * 	table to index into bitStripTable and pixelTable****
	 * 
	 * The encoded image data then follows ending in a 0x00 at the file length stored
	 * in the first two bytes of the file. there is then one extra byte holding a
	 * checksum produced by the addition of all the bytes in the file (except the first
	 * two and itself)*
	 * 
	 * [*] in some PC games the file is padded out beyond this length to the
	 * nearest 0x80/0x00 boundary with the byte 0x1A. The valid data in the
	 * file still finishes where this word says with the checkbyte following it.
	 * [**] I imagine when a game was running on a PC this standard palette
	 * was algorithimcally changed to suit the graphics mode being used
	 * (Hercules, MDA, CGA, EGA, MCGA, VGA &c.)
	 * [***]   Note also, in image 1 of PC Time & Magik I think one palette entry
	 * is bad as what should be white in the image is actually set to
	 * a very pale yellow. This is corrected with the display of the next
	 * sub-picture and I am pretty sure it is note a decoding problem
	 * here as when run on the PC the same image has the same pale yellow
	 * cast.
	 * [****] for detail of how all this works see below
	 * 
	 * As this file format is intended for two very different platforms the decoded
	 * imaged data is in a neutral, intermediate form. Each pixel is extracted as a
	 * byte with only the low four bits significant. The pixel value is an index into
	 * the sixteen entry palette.
	 * 
	 * The pixel data is compressed, presumably to allow a greater number of images
	 * to be distributed on the (rather small) default ST & PC floppy disks (in both
	 * cases about 370 Kbytes.)*****
	 * 
	 * Here's how to decode the data. The image data is actually a contiguous bit
	 * stream with the byte structure on disk having almost no relevance to the
	 * encoding. We access the bit stream via a two-byte buffer arranged as a word.
	 * 
	 * Preparation:
	 * 
	 * Initially, move the first byte from the image data into the low byte of
	 * theBitStreamBuffer and move the second byte of the image data into the
	 * high byte of theBitStreamBuffer.
	 * 
	 * Set a counter (theBufferBitCounter) to 8 which you will use to keep track
	 * of when it is necesary to refill the buffer.
	 * 
	 * Set a L9BYTE variable (theNewPixel) to byte 40 (seedByte) of the header.
	 * We need to do this because as part of identifying the pixel being
	 * extracted we need to know the value of the previous pixel extracted. Since
	 * none exists at this point we must prime this variable with the correct
	 * value.
	 * 
	 * Extraction:
	 * 
	 * Set up a loop which you will execute once for each pixel to be extracted
	 * and within that loop do as follows.
	 * 
	 * Copy the low byte of theBitStreamBuffer to an L9BYTE
	 * (theNewPixelIndexSelector). Examine theNewPixelIndexSelector. If this
	 * is 0xFF this flags that the index to the new pixel is present as a
	 * literal in the bit stream; if it is NOT 0xFF then the new pixel index
	 * value has to be decoded.
	 * 
	 * If theNewPixelIndexSelector is NOT 0xFF do as follows:
	 * 
	 * Set the variable theNewPixelIndex to the byte in the
	 * indexByteTable array of the header indexed by
	 * theNewPixelIndexSelector.
	 * 
	 * Set the variable theBufferBitStripCount to the value in the
	 * bitStripTable array of the header indexed by theNewPixelIndex.
	 * 
	 * One-by-one use right bit shift (>>) to remove
	 * theBufferBitStripCount bits from theBitStreamBuffer. After each
	 * shift decrement theBufferBitCounter and check whether it has
	 * reached 0. If it has, get the next byte from the image data and
	 * insert it in the high byte of theBitStreamBuffer and reset
	 * theBufferBitCounter to 8. What is happening here is as we remove
	 * each bit from the bottom of the bit stream buffer we check to see
	 * if there are any bits left in the high byte of the buffer. As soon
	 * as we know there are none, we refill it with the next eight bits
	 * from the image data.
	 * 
	 * When this 'bit-stripping' is finished, other than actually identifying
	 * the new pixel we are nearly done. I will leave that for the moment and
	 * look at what happens if the low byte of theBitStreamBuffer we put in
	 * theNewPixelIndexSelector was actually 0xFF:
	 * 
	 * In this case, instead of the above routine we begin by removing
	 * the low eight bits from the theBitStreamBuffer. We use the same
	 * ono-by-one bit shift right process described above to do this,
	 * again checking after each shift if it is necesary to refill the
	 * buffer's high byte.
	 * 
	 * When the eight bits have been removed we set theNewPixelIndex to
	 * the value of the low four bits of theBitStreamBuffer. Having done
	 * that we again one-by-one strip off those low four bits from the
	 * theBitStreamBuffer, again checking if we need to refill the buffer
	 * high byte.
	 * 
	 * Irrespective of whether we initially had 0xFF in
	 * theNewPixelIndexSelector we now have a new value in theNewPixelIndex.
	 * This value is used as follows to obtain the new pixel value.
	 * 
	 * The variable theNewPixel contains either the seedByte or the value of
	 * the previously extracted pixel. In either case this is a 4-bit value
	 * in the lower 4 bits. Use the left bit shift operator (or multiply by
	 * 16) to shift those four bits into the high four bits of theNewPixel.
	 * 
	 * Add the value in theNewPixelIndex (it is a 4-bit value) to
	 * theNewPixel. The resulting value is used as an index into the
	 * pixelTable array of the header to get the actual new pixel value so
	 * theNewPixel = header.pixelTable[theNewPixel] gets us our new pixel and
	 * primes theNewPixel for the same process next time around the loop.
	 * 
	 * Having got our new pixel it is stored in the next empty space in the
	 * bitmap and we loop back and start again.
	 * 
	 * [*****] I am not sure how the compression was done - someone with a better
	 * understanding of this area may be able to work out the method from the above.
	 * I worked out how to decode it by spending many, many hours tracing through the
	 * code in a debugger - thanks to the now defunct HiSoft for their DevPac ST and
	 * Gerin Philippe for NoSTalgia <http://users.skynet.be/sky39147/>.
	 */
	static bool bitmap_pc2_decode(Bitmap &bitmap, const Common::String &file, int x, int y);
	static BitmapType bitmap_pc_type(const Common::String &file);

	//@}

	/**
	 * @defgroup ST Bitmaps
	 *
	 * On the ST different graphics file formats were used for the early V4
	 * games (Knight Orc, Gnome Ranger) and the later V4 games (Lancelot,
	 * Ingrid's Back, Time & Magik and Scapeghost).
	 */
	//@{

	/**
	 * Extracts the number of pixels requested from an eight-byte data block (4 bit-
	 * planes) passed to it.
	 * 
	 * Note:   On entry each one of four pointers is set to point to the start of each
	 * 		bit-plane in the block. The function then indexes through each byte in
	 * 		each bit plane. and uses shift and mask operations to extract each four
	 * 		bit pixel into an L9PIXEL.
	 * 
	 * 		The bit belonging to the pixel in the current byte of the current bit-
	 * 		plane is moved to its position in an eight-bit pixel. The byte is then
	 * 		masked by a value to select only that bit and added to the final pixel
	 * 		value.
	*/
	static uint32 bitmap_st1_decode_pixels(byte *pic, byte *data, uint32 count, uint32 pixels);

	/**
	 * The ST image file has the following format. It consists of a 44 byte header
	 * followed by the image data.
	 * 
	 * The header has the following format:
	 * Bytes 0-31:     sixteen entry ST palette
	 * Bytes 32-33:    padding
	 * Bytes 34-35:    big-endian word holding number of bitplanes needed to make
	 * 			up a row of pixels*
	 * Bytes 36-37:    padding
	 * Bytes 38-39:    big-endian word holding number of rows in the image*
	 * Bytes 40-41:    padding**
	 * Bytes 42-43:    mask for pixels to show in last 16 pixel block. Again, this
	 * 			is big endian
	 * 
	 * [*]     these are probably big-endian unsigned longs but I have designated
	 * 		the upper two bytes as padding because (a) Level 9 does not need
	 * 		them as longs and (b) using unsigned shorts reduces byte sex induced
	 * 		byte order juggling.
	 * [**]    not certain what this is for but I suspect that, like bytes 42-43
	 * 		it is a mask to indicate which pixels to show, in this case in the
	 * 		first 16 pixel block
	 * 
	 * The image data is essentially a memory dump of the video RAM representing
	 * the image in lo-res mode. In lo-res mode each row is 320 pixels wide
	 * and each pixel can be any one of sixteen colours - needs 4 bits to store.
	 * 
	 * In the ST video memory (in lo-res mode which we are dealing with here)
	 * is organised as follows. The lowest point in memory in the frame buffer
	 * represents the top-left of the screen, the highest the bottom-right.
	 * Each row of pixels is stored in sequence.
	 * 
	 * Within each pixel row the pixels are stored as follows. Each row is
	 * divided into groups of 16 pixels. Each sixteen pixel group is stored
	 * in 8 bytes, logically four groups of two. Each two byte pair
	 * is a bit-plane for that sixteen pixel group - that is it stores the
	 * same bit of each pixel in that group. The first two bytes store the
	 * lowest bit, the second pair the second bit &c.
	 * 
	 * The word at bytes 34-35 of the header stores the number of bitplanes
	 * that make up each pixel row in the image. Multplying this number by
	 * four gives the number of pixels in the row***. For title and frame
	 * images that will be 320, for sub-images it will be less.
	 * 
	 * [***]   Not always exactly. For GnomeRanger sub-images this value is 60
	 * 		- implying there are 240 pixels per row. In fact there are only
	 * 		225 pixels in each row. To identify this situation look at the
	 * 		big-endian word in bytes 42-43 of the header. This is a mask
	 * 		telling you the pixels to use. Each bit represents one pixel in
	 * 		the block, with the MSBit representing the first pixel and the
	 * 		LSbit the last.
	 * 
	 * 		In this situation, the file does contain the entire sixteen
	 * 		pixel block (it has to with the bitplane arrangement) but
	 * 		the pixels which are not part of the image are just noise. When
	 * 		decoding the image, the L9BITMAP produced has the actual pixel
	 * 		dimensions - the surplus pixels are discarded.
	 * 
	 * 		I suspect, though I have not found an instance, that in theory
	 * 		the same situation could apply at the start of a pixel row and that
	 * 		in this case the big-endian word at bytes 40-41 is the mask.
	 * 
	 * Having obtained the pixel dimensions of the image the function uses
	 * them to allocate memory for the bitmap and then extracts the pixel
	 * information from the bitmap row by row. For each row eight byte blocks
	 * are read from the image data and passed to UnpackSTv1Pixels along with
	 * the number of pixels to extract (usually 16, possibly less for the last
	 * block in a row.)
	 */
	static bool bitmap_st1_decode(Bitmap &bitmap, const Common::String &file, int x, int y);
	static Common::String bitmap_st2_name(int num);

	//@}

	/**
	 * @defgroup Amiga Bitmaps
     */
	//@{

	static int bitmap_amiga_intensity(int col) {
		return (int)(pow((double)col / 15, 1.0 / 0.8) * 0xff);
	}

	/**
	 * Amiga palette colours are word length structures with the red, green and blue
	 * values stored in the second, third and lowest nybles respectively. The high
	 * nybble is always zero.
	*/
	static Colour bitmap_amiga_colour(int i1, int i2);

	/**
	 * The Amiga image file has the following format. It consists of a 44 byte
	 * header followed by the image data.
	 * 
	 * The header has the following format:
	 * Bytes 0-63:  thirty-two entry Amiga palette
	 * Bytes 64-65: padding
	 * Bytes 66-67: big-endian word holding picture width in pixels*
	 * Bytes 68-69: padding
	 * Bytes 70-71: big-endian word holding number of pixel rows in the image*
	 * 
	 * [*] these are probably big-endian unsigned longs but I have designated
	 * the upper two bytes as padding because (a) Level 9 does not need
	 * them as longs and (b) using unsigned shorts reduces byte sex induced
	 * byte order juggling.
	 * 
	 * The images are designed for an Amiga low-res mode screen - that is they
	 * assume a 320*256 (or 320 * 200 if NSTC display) screen with a palette of
	 * 32 colours from the possible 4096.
	 * 
	 * The image data is organised the same way that Amiga video memory is. The
	 * entire data block is divided into five equal length bit planes with the
	 * first bit plane holding the low bit of each 5-bit pixel, the second bitplane
	 * the second bit of the pixel and so on up to the fifth bit plane holding the
	 * high bit of the f5-bit pixel.
	*/
	static bool bitmap_amiga_decode(Bitmap &bitmap, const Common::String &file, int x, int y);

	//@}

	/**
	 * @defgroup Macintosh Bitmaps
     */
	//@{

	/**
	 * The Mac image file format is very simple. The header is ten bytes
	 * with the width of the image in pixels in the first long and the
	 * height (in pixel rows) in the second long - both are big-endian.
	 * (In both cases I treat these as unsigned shorts to minimise byte
	 * twiddling when working around byte sex issues). There follow two
	 * unidentified bytes - possibly image type identifiers or maybe
	 * valid pixel masks for the beginning and end of pixel rows in
	 * sub-images.
	 * 
	 * The image data is extremely simple. The entire block is a packed array
	 * of 1-bit pixels - I.E. each byte holds eight pixels - with 1 representing
	 * white and 0 representing black. The pixels are organised with the top
	 * left first and bottom left last, each row in turn.
	 * 
	 * The image sizes are 512 * 216 pixels for main images and 360 * 186 pixels
	 * for sub-images.
	 */
	static bool bitmap_mac_decode(Bitmap &bitmap, const Common::String &file, int x, int y);

	//@}

	/**
	 * @defgroup C64 Bitmaps, also related formats (BBC B, Amstrad CPC and Spectrum +3)
	 */
	//@{


	static Common::String bitmap_c64_name(int num);
	static Common::String bitmap_bbc_name(int num);
	static Common::String bitmap_cpc_name(int num);
	static BitmapType bitmap_c64_type(const Common::String &file);

	/**
	 * The C64 graphics file format is (loosely) based on the layout of
	 * C64 graphics memory. There are in fact two formats (i) the
	 * standard game images and (ii) title pictures. For both formats
	 * the file begins within the 2-byte pair 0x00 and 0x20.
	 * 
	 * The images are "multi-color bitmap mode" images which means they
	 * have rows of 160 double width pixels and can be up to 200 rows
	 * long. (The title images are 200 lines long, the game images are
	 * 136 lines long.) Unlike Amiga, Mac, ST and PC graphics there are
	 * no "main" and "sub" images. All game graphics have the same
	 * dimensions and each completely replaces its predecessor.
	 * 
	 * The graphics files used on the Amstrad CPC and Spectrum +3 are also
	 * virtually identical to C64 graphics files. This choice was presumably
	 * made because although the CPC screen was more capable than the c64 it
	 * was (in low resolution) the same size (160*200) and presumably
	 * algorothmic conversion conversion of the colours was trivial for
	 * the interpreter. In addition (a) the artwork already existed so no
	 * extra expense would be incurred and (b) by accepting the C64's
	 * limitation of only four colours in each 4*8 pixel block (but still
	 * with sixteen colours on screen) they got a compressed file format
	 * allowing more pictures on each disk.
	 * 
	 * The file organisation is rather different though. Only picture
	 * one and the title picture are separate files. All the other
	 * pictures (2-29) are stored in one large file "allpics.pic".
	 * 
	 * On these platforms the picture 1 file and title picture file have
	 * an AMSDOS header (a 128 byte block of metadata) which contains a
	 * checksum of the first 66 bytes of the header in a little-endian
	 * word at bytes 67 & 68. On the original C64 platform there was a
	 * simple two byte header. Following the header the data is organised
	 * exactly as in the C64 game and title image files. The
	 * 'allpics.pic" file has no header and consists of 0x139E blocks
	 * each forming a picture, in the C64 game file format (minus the two
	 * byte header).
	 */
	static bool bitmap_c64_decode(Bitmap &bitmap, const Common::String &file, BitmapType type, int num);

	//@}

	/**
	 * The graphics files used by the BBC B are virtually identical
	 * to C64 graphics files. I assume that (as with the CPC and
	 * Spectrum+3) this choice was made because the BBC mode 2 screen,
	 * was nearly the same size (160*256) and had roughly the same capability
	 * as the C64 screen (displays 16 colours, although eight of those ar
	 * just the first eight flashing).
	 * 
	 * In addition (a) the artwork already existed so no extra expense would
	 * be incurred and (b) by accepting the C64's limitation of only four
	 * colours in each 4*8 pixel block (but still with sixteen colours on
	 * screen) they got a compressed file format allowing more pictures
	 * on each disk.
	 * 
	 * The file organisation is very close to the C64. The naming system
	 * can be the same eg "PIC12", but another form is also used :
	 * "P.Pic12". Unlike the C64 the BBC has well defined title images,
	 * called "TITLE" or P.Title. All pictures are in separate files.
	 * 
	 * The only difference seems to be:
	 * 
	 * * There is either *no* header before the image data or a simple
	 * 10 byte header which I think *may* be a file system header
	 * left in place by the extractor system.
	 * 
	 * * There is an extra 32 bytes following the data at the end of
	 * each file. These bytes encode a table to convert between the 16
	 * C64 colours and 16, four-pixel pix-patterns used to let the BBC
	 * (with only 8 colours) represent the sixteen possible C64 colours.
	 * 
	 * A pix-pattern looks like this:
	 * 
	 * 		 |   Even |   Odd   |
	 * 		 | Column | Column  |
	 * 	-----------------------------
	 * 	Even Row |Pixel 1 | Pixel 2 |
	 * 	---------|--------|---------|
	 * 	Odd Row  |Pixel 3 | Pixel 4 |
	 * 	-----------------------------
	 * 
	 * Each of the four pixel *can* be any of the eight BBC Mode 2
	 * steady colours. In practice they seem either to be all the
	 * same or a simple check of two colours - the pixels in the
	 * odd row being in the reverse order to those in the even row.
	 * 
	 * When converting a C64 pixel to a BBC pixel the game uses the
	 * value of the C64 pixel as an index into the array of sixteen
	 * BBC pix-patterns. The game looks at the selected pattern and
	 * chooses the BBC pixel colour thus: if the pixel is in an even
	 * numbered row and an even numbered column, it uses Pixel 1 from
	 * the pattern, if in an even row but an odd column, it uses Pixel 3
	 * and so on.
	 * 
	 * The pix-pattern data is encoded thus: the first sixteen bytes
	 * encode the even row pixels for the patterns, one byte per
	 * pattern, and in the same way the second sixteen bytes encode
	 * the odd row pixels for each pattern. For example for the
	 * pattern representing C64 colour 0 the even row pixels are encoded
	 * in the first byte and the odd row pixels in the sixteenth byte.
	 * 
	 * Within each byte the pixels are encoded in this way:
	 * 
	 * Bit     7   6   5   4   3   2   1   0
	 * -------------------------------------
	 * 		0   0   1   0   0   1   1   1
	 * 		|   |   |   |   |   |   |   |
	 * 		+---|---+---|---+---|---+---|----- Even Pixel 0101 (5)
	 * 			|       |       |       |
	 * 			+-------+-------+-------+----- Odd Pixel 0011 (3)
	 * 
	 * This function calls the C64 decoding routines to do the actual
	 * loading. See the comments to that function for details of how the
	 * image is encoded and stored.
	 */
	static bool bitmap_bbc_decode(Bitmap &bitmap, const Common::String &file, BitmapType type, int num);
private:
	/**
	 * Returns a picture number from a valid passed
	 */
	int getPictureNumber(const Common::String &filename) const;

	/**
	 * Decode a picture
	 */
	static bool decodeBitmap(Bitmap &bitmap, int num);
public:
	static BitmapType _bitmapType;

	/**
	 * Checks what type of graphics, if any, the game supports, setting _bitmapType.
	 * Returns true if graphics are present
	 */
	static bool exists();

	/**
	 * Returns the number of colors and dimensions of a given picture
	 */
	static void getPicInfo(int picNum, uint16 &width, uint16 &height, int &colorCount);

	/**
	 * Returns true if there are graphics
	 */
	static bool hasGraphics() { return _bitmapType != NO_BITMAPS; }
public:
	/**
	 * Constructor
	 */
	BitmapFileSystem();

	/**
	 * Check if a member with the given name is present in the Archive.
	 * Patterns are not allowed, as this is meant to be a quick File::exists()
	 * replacement.
	 */
	virtual bool hasFile(const Common::String &name) const override;

	/**
	 * Add all members of the Archive to list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return the number of names added to list
	 */
	virtual int listMembers(Common::ArchiveMemberList &list) const override;

	/**
	 * Returns a ArchiveMember representation of the given file.
	 */
	virtual const Common::ArchiveMemberPtr getMember(const Common::String &name) const override;

	/**
	 * Create a stream bound to a member with the specified name in the
	 * archive. If no member with this name exists, 0 is returned.
	 * @return the newly created input stream
	 */
	virtual Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const override;
};

} // End of namespace Level9
} // End of namespace Glk

#endif

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

#include "ultima/games/ultima1/u1gfx/view_title.h"
#include "ultima/games/ultima1/u1gfx/drawing_support.h"
#include "ultima/games/ultima1/core/resources.h"
#include "ultima/games/ultima1/game.h"
#include "ultima/core/file.h"
#include "ultima/messages.h"

namespace Ultima {
namespace Ultima1 {
namespace U1Gfx {

BEGIN_MESSAGE_MAP(ViewTitle, Gfx::VisualContainer)
	ON_MESSAGE(KeypressMsg)
	ON_MESSAGE(FrameMsg)
END_MESSAGE_MAP()

void load16(Graphics::ManagedSurface &s, Common::ReadStream &in) {
	byte *destP = (byte *)s.getPixels();
	byte v;
	for (int idx = 0; idx < (s.w * s.h); idx += 2) {
		v = in.readByte();
		*destP++ = v & 0xf;
		*destP++ = v >> 4;
	}
}

ViewTitle::ViewTitle(TreeItem *parent) : Gfx::VisualContainer("Title", Rect(0, 0, 320, 200), parent),
		_mode(TITLEMODE_COPYRIGHT), _counter(0) {
	_expiryTime = getGame()->getMillis() + 2000;

	// Load the Origin logo
	File f("ULTIMA1/LOGO");
	_logo.create(275, 64);
	f.read((byte *)_logo.getPixels(), _logo.w * _logo.h);
	f.close();

	// Load the Ultima castle bitmap
	f.open("castle.16");
	_castle.create(320, 200);
	load16(_castle, f);
	f.close();

	// Load the flags
	f.open("ULTIMA1/FLAGS");
	for (int idx = 0; idx < 3; ++idx) {
		_flags[idx].create(8, 8);
		f.read(_flags[idx].getPixels(), 64);
	}
	f.close();
}

ViewTitle::~ViewTitle() {
}

void ViewTitle::draw() {
	switch (_mode) {
	case TITLEMODE_COPYRIGHT:
		drawCopyrightView();
		break;

	case TITLEMODE_PRESENTS:
		drawPresentsView();
		break;

	case TITLEMODE_CASTLE:
		drawCastleView();
		break;

	case TITLEMODE_TRADEMARKS:
		drawTrademarksView();
		break;

	case TITLEMODE_MAIN_MENU:
		drawMainMenu();
		break;

	default:
		break;
	}
}

void ViewTitle::drawCopyrightView() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Gfx::VisualSurface s = getSurface();
	s.clear();

	// Draw horizontal title lines
	for (int idx = 0; idx < 3; ++idx) {
		s.hLine(112, idx + 58, 200, 1);
		s.hLine(112, idx + 74, 200, 1);
	}

	// Write text
	s.writeString(game->_res->TITLE_MESSAGES[0], TextPoint(16, 8), game->_whiteColor);
	s.writeString(game->_res->TITLE_MESSAGES[1], TextPoint(8, 11), game->_whiteColor);
	s.writeString(game->_res->TITLE_MESSAGES[2], TextPoint(0, 21), game->_whiteColor);
}

void ViewTitle::drawPresentsView() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Gfx::VisualSurface s = getSurface();
	s.clear();
	s.blitFrom(_logo, Point(20, 21));

	switch (_counter) {
	case 0:
		s.writeString(game->_res->TITLE_MESSAGES[3], TextPoint(14, 13), game->_textColor);
		break;
	case 1:
		s.writeString(game->_res->TITLE_MESSAGES[4], TextPoint(5, 12), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[5], TextPoint(5, 13), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[6], TextPoint(5, 14), game->_textColor);
		break;
	case 2:
		s.writeString(game->_res->TITLE_MESSAGES[7], TextPoint(6, 12), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[8], TextPoint(6, 13), game->_textColor);
		break;
	default:
		break;
	}
}

void ViewTitle::drawCastleView() {
	Gfx::VisualSurface s = getSurface();
	if (_counter == 0)
		s.blitFrom(_castle);

	drawCastleFlag(s, 123);
	drawCastleFlag(s, 196);
}

void ViewTitle::drawCastleFlag(Gfx::VisualSurface &s, int xp) {
	s.blitFrom(_flags[getGame()->getRandomNumber(0, 2)], Common::Point(xp, 55));
}

void ViewTitle::drawTrademarksView() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Gfx::VisualSurface s = getSurface();
	if (_counter == 0)
		s.clear();

	if (_counter < 32) {
		s.blitFrom(_logo, Common::Rect(0, 0, _logo.w, _counter + 1), Point(20, 21));
		s.blitFrom(_logo, Common::Rect(0, _logo.h - _counter - 1, _logo.w, _logo.h),
			Common::Point(20, 21 + _logo.h - _counter - 1));
	} else {
		s.writeString(game->_res->TITLE_MESSAGES[9], TextPoint(1, 17), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[10], TextPoint(2, 18), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[11], TextPoint(11, 19), game->_textColor);
		s.writeString(game->_res->TITLE_MESSAGES[12], TextPoint(6, 23), game->_textColor);
	}	
}

void ViewTitle::drawMainMenu() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Gfx::VisualSurface s = getSurface();
	DrawingSupport ds(s);
	s.clear();
	ds.drawFrame();

	s.writeString(game->_res->MAIN_MENU_TEXT[0], TextPoint(12, 6), game->_whiteColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[1], TextPoint(14, 9), game->_whiteColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[2], TextPoint(14, 10), game->_whiteColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[3], TextPoint(13, 11), game->_whiteColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[4], TextPoint(8, 14), game->_textColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[5], TextPoint(8, 15), game->_textColor);
	s.writeString(game->_res->MAIN_MENU_TEXT[6], TextPoint(13, 18), game->_textColor);
}

void ViewTitle::setTitlePalette() {
	const byte PALETTE[] = { 0, 1, 2, 3, 4, 5, 6, 7, 56, 57, 58, 59, 60, 61, 62, 63 };
	getGame()->setEGAPalette(PALETTE);
}

void ViewTitle::setCastlePalette() {
	const byte PALETTE[] = { 0, 24, 7, 63, 63, 34, 58, 14, 20, 7, 61, 59, 1, 57, 7, 63 };
	getGame()->setEGAPalette(PALETTE);
}

bool ViewTitle::FrameMsg(CFrameMsg &msg) {
	uint32 time = getGame()->getMillis();

	switch (_mode) {
	case TITLEMODE_COPYRIGHT:
		if (time > _expiryTime) {
			_mode = TITLEMODE_PRESENTS;
			_counter = 0;
			_expiryTime = time + 3000;
		}
		break;

	case TITLEMODE_PRESENTS:
		if (time > _expiryTime) {
			_expiryTime = time + 3000;
			if (++_counter == 3) {
				_mode = TITLEMODE_CASTLE;
				_counter = 0;
				_expiryTime = time;
				setCastlePalette();
			}
		}
		break;

	case TITLEMODE_CASTLE:
		_expiryTime = time + 20;
		if (++_counter == 100) {
			_mode = TITLEMODE_PRESENTS;
			_counter = 0;
			_expiryTime = time + 3000;
			setTitlePalette();
		}
		break;

	case TITLEMODE_TRADEMARKS:
		if (time > _expiryTime) {
			_expiryTime = time + 20;
			++_counter;
			if (_counter == 32) {
				_expiryTime = time + 4000;
			} else if (_counter == 33) {
				_mode = TITLEMODE_MAIN_MENU;
				_expiryTime = time;
				_counter = 0;
			}
		}
		break;

	default:
		break;
	}

	return true;
}

bool ViewTitle::KeypressMsg(CKeypressMsg &msg) {
	uint32 time = getGame()->getMillis();

	if (_mode == TITLEMODE_MAIN_MENU) {

	} else if (_mode != TITLEMODE_TRADEMARKS) {
		// Switch to the trademarks view
		_mode = TITLEMODE_TRADEMARKS;
		_expiryTime = time;
		_counter = -1;
	}

	return true;
}

} // End of namespace U1Gfx
} // End of namespace Shared
} // End of namespace Ultima

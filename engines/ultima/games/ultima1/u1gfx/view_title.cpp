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

ViewTitle::ViewTitle(TreeItem *parent) : Gfx::VisualContainer("Title", Rect(0, 0, 320, 200), parent),
		_mode(TITLEMODE_COPYRIGHT), _counter(0) {
	_expiryTime = getGame()->getMillis() + 2000;

	// Load the Origin logo
	File f("ULTIMA1/LOGO");
	_logo.create(275, 64);
	f.read((byte *)_logo.getPixels(), _logo.w * _logo.h);
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

	default:
		drawCastleView();
		break;
	}


}

void ViewTitle::drawCopyrightView() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Gfx::VisualSurface s = getSurface();

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

	}

}

void ViewTitle::drawCastleView() {
	Gfx::VisualSurface s = getSurface();

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
			}
		}
		break;

	default:
		break;
	}

	return true;
}

bool ViewTitle::KeypressMsg(CKeypressMsg &msg) {
	return true;
}

} // End of namespace U1Gfx
} // End of namespace Shared
} // End of namespace Ultima

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

#include "legend/later/later_screen.h"
#include "engines/util.h"

namespace Legend {
namespace Later {

LaterScreen::LaterScreen(LegendEngine *vm) : Screen(vm) {
}

LaterScreen::~LaterScreen() {
}

void LaterScreen::setupGraphics() {
	initGraphics(640, 480, false);
}

void LaterScreen::setDefaultPalette() {
	for (int idx = 0; idx < 48; ++idx) {
		_gamePalette[idx] = ((idx / 3) * 63) / 15;
	}
	_gamePalette[PALETTE_SIZE - 3] = 0;
	_gamePalette[PALETTE_SIZE - 2] = 0;
	_gamePalette[PALETTE_SIZE - 1] = 0;

	setPalette(_gamePalette);
}

} // End of namespace Later
} // End of namespace Legend

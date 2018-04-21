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

#include "ultima/ultima.h"
#include "ultima/game_base.h"
#include "ultima/games/shared/core/game_state.h"
#include "ultima/games/ultima1/game.h"
#include "ultima/games/ultima1/core/map.h"

namespace Ultima {
namespace Shared {

GameState::GameState(Game *game) : MAX_TILES_PER_ORIGINAL(16, 16), _videoMode(EGA) {
	_characters.resize(1);
	_gameId = g_vm->getGameID();

	switch (_gameId) {
	case GType_Ultima1:
		_map = new Ultima1::Ultima1Map(static_cast<Ultima1::Ultima1Game *>(game));
		break;
	default:
		error("Unspported game");
		break;
	}

	// Load the default overworld map
	_map->loadMap(0, _videoMode);
	_map->setPosition(Point(49, 40));
}

GameState::~GameState() {
	delete _map;
}

} // End of namespace Shared
} // End of namespace Ultima

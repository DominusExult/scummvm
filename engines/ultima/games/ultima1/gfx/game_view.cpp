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

#include "ultima/games/ultima1/gfx/game_view.h"
#include "ultima/games/shared/core/map.h"
#include "ultima/games/shared/gfx/info.h"
#include "ultima/games/ultima1/game.h"
#include "ultima/games/ultima1/gfx/drawing_support.h"
#include "ultima/games/ultima1/gfx/status.h"
#include "ultima/games/ultima1/gfx/viewport_dungeon.h"
#include "ultima/games/ultima1/gfx/viewport_map.h"
#include "ultima/games/ultima1/actions/move.h"
#include "ultima/games/ultima1/actions/climb.h"
#include "ultima/games/ultima1/actions/enter.h"
#include "ultima/games/ultima1/core/resources.h"
#include "ultima/messages.h"

namespace Ultima {
namespace Ultima1 {
namespace U1Gfx {

BEGIN_MESSAGE_MAP(GameView, Gfx::VisualContainer)
	ON_MESSAGE(KeypressMsg)
END_MESSAGE_MAP()

GameView::GameView(TreeItem *parent) : Gfx::VisualContainer("GameView", Rect(0, 0, 320, 200), parent) {
	_info = new Shared::Info(this);
	_status = new Status(this);
	_viewportDungeon = new ViewportDungeon(this);
	_viewportMap = new ViewportMap(this);
	_actions[0] = new Actions::Move(this);
	_actions[1] = new Actions::Climb(this);
	_actions[2] = new Actions::Enter(this);
}

GameView::~GameView() {
	delete _info;
	delete _status;
	delete _viewportDungeon;
	delete _viewportMap;
	for (int idx = 0; idx < 3; ++idx)
		delete _actions[idx];
}

void GameView::draw() {
	DrawingSupport ds(getSurface());
	ds.drawGameFrame();
	drawIndicators();

	_info->draw();
	_status->draw();
	
	Ultima1Map *map = static_cast<Ultima1Map *>(getMap());
	switch (map->_mapType) {
	case MAP_DUNGEON:
		_viewportDungeon->draw();
		break;
	default:
		_viewportMap->draw();
		break;
	}
}

void GameView::drawIndicators() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Ultima1Map *map = static_cast<Ultima1Map *>(getMap());

	Gfx::VisualSurface s = getSurface();
	DrawingSupport ds(s);

	if (map->_mapType == MAP_DUNGEON) {
		// Draw the dungeon level indicator
		ds.drawRightArrow(TextPoint(15, 0));
		s.writeString(game->_res->DUNGEON_LEVEL, TextPoint(16, 0), game->_textColor);
		s.writeString(Common::String::format("%2d", map->_dungeonLevel), TextPoint(23, 0), game->_textColor);
		ds.drawLeftArrow(TextPoint(26, 0));

		// Draw the current direction
		const char *dir = game->_res->DIRECTION_NAMES[map->_direction - 1];
		ds.drawRightArrow(TextPoint(16, 19));
		s.writeString("       ", TextPoint(17, 19), game->_textColor);
		s.writeString(dir, TextPoint(19 - (7 - strlen(dir)) / 2, 19), game->_textColor);
		ds.drawLeftArrow(TextPoint(24, 19));
	}
}

bool GameView::KeypressMsg(CKeypressMsg &msg) {
	if (msg._keyState.keycode == Common::KEYCODE_LEFT || msg._keyState.keycode == Common::KEYCODE_KP4) {
		CMoveMsg move(Shared::DIR_LEFT);
		move.execute(this);
	} else if (msg._keyState.keycode == Common::KEYCODE_RIGHT || msg._keyState.keycode == Common::KEYCODE_KP6) {
		CMoveMsg move(Shared::DIR_RIGHT);
		move.execute(this);
	} else if (msg._keyState.keycode == Common::KEYCODE_UP || msg._keyState.keycode == Common::KEYCODE_KP8) {
		CMoveMsg move(Shared::DIR_UP);
		move.execute(this);
	} else if (msg._keyState.keycode == Common::KEYCODE_DOWN || msg._keyState.keycode == Common::KEYCODE_KP2) {
		CMoveMsg move(Shared::DIR_DOWN);
		move.execute(this);
	} else if (msg._keyState.keycode == Common::KEYCODE_e) {
		CEnterMsg enter;
		enter.execute(this);
	} else if (msg._keyState.keycode == Common::KEYCODE_k) {
		CClimbMsg climb;
		climb.execute(this);
	} else {
		return false;
	}

	return true;
}

} // End of namespace U1Gfx
} // End of namespace Shared
} // End of namespace Ultima

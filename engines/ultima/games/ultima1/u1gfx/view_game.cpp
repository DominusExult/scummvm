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

#include "ultima/games/ultima1/u1gfx/view_game.h"
#include "ultima/games/shared/actions/huh.h"
#include "ultima/games/shared/actions/pass.h"
#include "ultima/games/shared/maps/map.h"
#include "ultima/games/ultima1/game.h"
#include "ultima/games/ultima1/u1gfx/drawing_support.h"
#include "ultima/games/ultima1/u1gfx/info.h"
#include "ultima/games/ultima1/u1gfx/status.h"
#include "ultima/games/ultima1/u1gfx/viewport_dungeon.h"
#include "ultima/games/ultima1/u1gfx/viewport_map.h"
#include "ultima/games/ultima1/actions/move.h"
#include "ultima/games/ultima1/actions/map_action.h"
#include "ultima/games/ultima1/actions/stats.h"
#include "ultima/games/ultima1/core/resources.h"
#include "ultima/gfx/text_cursor.h"
#include "ultima/messages.h"

namespace Ultima {
namespace Ultima1 {
namespace Actions {
	MAP_ACTION(Drop, 3, drop);
	MAP_ACTION(Enter, 4, enter);
	MAP_ACTION(Get, 6, get);
	MAP_ACTION(HyperJump, 7, hyperjump);
	MAP_ACTION(Inform, 8, inform);
	MAP_ACTION(Climb, 10, climb);
	MAP_ACTION(Open, 14, open);
	MAP_ACTION(Steal, 18, steal);
	MAP_ACTION(Transact, 19, talk)
	MAP_ACTION(Unlock, 20, unlock)
	MAP_ACTION(ViewChange, 21, view)
	MAP_ACTION(ExitTransport, 23, disembark)
}
	
namespace U1Gfx {

BEGIN_MESSAGE_MAP(ViewGame, Gfx::VisualContainer)
	ON_MESSAGE(FrameMsg)
	ON_MESSAGE(KeypressMsg)
END_MESSAGE_MAP()

ViewGame::ViewGame(TreeItem *parent) : Gfx::VisualContainer("Game", Rect(0, 0, 320, 200), parent), _frameCtr(0) {
	_info = new Info(this);
	_status = new Status(this);
	_viewportDungeon = new ViewportDungeon(this);

	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	_viewportMap = new ViewportMap(this);
	
	_actions.resize(16);
	_actions[0] = new Actions::Move(this);
	_actions[1] = new Shared::Actions::Huh(this, game->_res->HUH);
	_actions[2] = new Actions::Drop(this);
	_actions[3] = new Actions::Enter(this);
	_actions[4] = new Actions::Get(this);
	_actions[5] = new Actions::HyperJump(this);
	_actions[6] = new Actions::Inform(this);
	_actions[7] = new Actions::Climb(this);
	_actions[8] = new Actions::Open(this);
	_actions[9] = new Shared::Actions::Pass(this, game->_res->ACTION_NAMES[15]);
	_actions[10] = new Actions::Steal(this);
	_actions[11] = new Actions::Transact(this);
	_actions[12] = new Actions::Unlock(this);
	_actions[13] = new Actions::ViewChange(this);
	_actions[14] = new Actions::ExitTransport(this);
	_actions[15] = new Actions::Stats(this);
}

ViewGame::~ViewGame() {
	delete _info;
	delete _status;
	delete _viewportDungeon;
	delete _viewportMap;
	for (uint idx = 0; idx < _actions.size(); ++idx)
		delete _actions[idx];
}

void ViewGame::draw() {
	Gfx::VisualSurface s = getSurface();

	if (_isDirty) {
		// Draw the overal frame
		s.clear();
		DrawingSupport ds(s);
		ds.drawGameFrame();
		drawIndicators();

		setDirty();
	}

	if (_info->isDirty())
		_info->draw();
	if (_status->isDirty())
		_status->draw();

	Maps::Ultima1Map *map = static_cast<Maps::Ultima1Map *>(getGame()->getMap());
	switch (map->_mapType) {
	case Maps::MAP_DUNGEON:
		_viewportDungeon->draw();
		break;
	default:
		_viewportMap->draw();
		break;
	}

	_isDirty = false;
}

void ViewGame::drawIndicators() {
	Ultima1Game *game = static_cast<Ultima1Game *>(getGame());
	Maps::Ultima1Map *map = static_cast<Maps::Ultima1Map *>(game->getMap());

	Gfx::VisualSurface s = getSurface();
	DrawingSupport ds(s);

	if (map->_mapType == Maps::MAP_DUNGEON) {
		// Draw the dungeon level indicator
		ds.drawRightArrow(TextPoint(15, 0));
		s.writeString(game->_res->DUNGEON_LEVEL, TextPoint(16, 0));
		s.writeString(Common::String::format("%2d", map->getLevel()), TextPoint(23, 0));
		ds.drawLeftArrow(TextPoint(26, 0));

		// Draw the current direction
		const char *dir = game->_res->DIRECTION_NAMES[map->getDirection() - 1];
		ds.drawRightArrow(TextPoint(16, 19));
		s.writeString("       ", TextPoint(17, 19));
		s.writeString(dir, TextPoint(19 - (7 - strlen(dir)) / 2, 19));
		ds.drawLeftArrow(TextPoint(24, 19));
	}
}

#define FRAME_REDUCTION_RATE 5

bool ViewGame::FrameMsg(CFrameMsg &msg) {
	if (_frameCtr == FRAME_REDUCTION_RATE) {
		// Ignore frame message at the start of passing reduced frame rate to child views
		return false;
	} else if (++_frameCtr == FRAME_REDUCTION_RATE) {
		msg.execute(this, nullptr, MSGFLAG_SCAN);
		_frameCtr = 0;
	}

	return true;
}

bool ViewGame::KeypressMsg(CKeypressMsg &msg) {
	getGame()->_textCursor->setVisible(false);

	switch (msg._keyState.keycode) {
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_KP4: {
		CMoveMsg move(Shared::Maps::DIR_LEFT);
		move.execute(this);
		break;
	}
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_KP6: {
		CMoveMsg move(Shared::Maps::DIR_RIGHT);
		move.execute(this);
		break;
	}
	case Common::KEYCODE_UP:
	case Common::KEYCODE_KP8: {
		CMoveMsg move(Shared::Maps::DIR_UP);
		move.execute(this);
		break;
	}
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_KP2: {
		CMoveMsg move(Shared::Maps::DIR_DOWN);
		move.execute(this);
		break;
	}
	case Common::KEYCODE_d: {
		CDropMsg drop;
		drop.execute(this);
		break;
	}
	case Common::KEYCODE_e: {
		CEnterMsg enter;
		enter.execute(this);
		break;
	}
	case Common::KEYCODE_g: {
		CGetMsg get;
		get.execute(this);
		break;
	}
	case Common::KEYCODE_h: {
		CHyperJumpMsg hyperjump;
		hyperjump.execute(this);
		break;
	}
	case Common::KEYCODE_i: {
		CInformMsg inform;
		inform.execute(this);
		break;
	}
	case Common::KEYCODE_k: {
		CClimbMsg climb;
		climb.execute(this);
		break;
	}
	case Common::KEYCODE_o: {
		COpenMsg open;
		open.execute(this);
		break;
	}
	case Common::KEYCODE_s: {
		CStealMsg steal;
		steal.execute(this);
		break;
	}
	case Common::KEYCODE_t: {
		CTransactMsg transact;
		transact.execute(this);
		break;
	}
	case Common::KEYCODE_u: {
		CUnlockMsg unlock;
		unlock.execute(this);
		break;
	}
	case Common::KEYCODE_v: {
		CViewChangeMsg view;
		view.execute(this);
		break;
	}
	case Common::KEYCODE_x: {
		CExitTransportMsg exit;
		exit.execute(this);
		break;
	}
	case Common::KEYCODE_z: {
		CStatsMsg stats;
		stats.execute(this);
		break;
	}
	case Common::KEYCODE_SPACE: {
		CPassMsg pass;
		pass.execute(this);
		break;
	}
	default: {
		CHuhMsg huh;
		huh.execute(this);
		break;
	}
	}

	// End of turn
	getGame()->endOfTurn();

	return true;
}

} // End of namespace U1Gfx
} // End of namespace Shared
} // End of namespace Ultima

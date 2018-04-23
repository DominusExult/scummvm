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

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "ultima/ultima.h"
#include "ultima/debugger.h"
#include "ultima/events.h"
#include "ultima/core/resources.h"
#include "ultima/core/mouse_cursor.h"
#include "ultima/gfx/screen.h"
#include "ultima/games/ultima1/game.h"

namespace Ultima {

UltimaEngine *g_vm;

UltimaEngine::UltimaEngine(OSystem *syst, const UltimaGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc) {
	g_vm = this;
	_debugger = nullptr;
	_events = nullptr;
	_game = nullptr;
	_mouseCursor = nullptr;
	_screen = nullptr;
}

UltimaEngine::~UltimaEngine() {
	delete _debugger;
	delete _events;
	delete _game;
	delete _mouseCursor;
	delete _screen;
}

bool UltimaEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript,      "scripts", "Script debug level");

	Resources *res = new Resources();
	if (!res->setup()) {
		delete res;
		return false;
	}

	_debugger = Debugger::init(this);
	_events = new Events();
	_screen = new Gfx::Screen();
	_mouseCursor = new MouseCursor();

	// Create the game, and signal to it that the game is starting
	_game = createGame();
	_events->addTarget(_game);
	_game->starting();
	return true;
}

void UltimaEngine::deinitialize() {
}

Common::Error UltimaEngine::run() {
	// Initialize the engine
	if (!initialize())
		return Common::kUnknownError;

	playGame();

	deinitialize();
	return Common::kNoError;
}

void UltimaEngine::playGame() {
	while (!shouldQuit()) {
		_events->pollEventsAndWait();
	}
}

Shared::Game *UltimaEngine::createGame() const {
	switch (getGameID()) {
	case GType_Ultima1:
		return new Ultima1::Ultima1Game();
	default:
		error("Unknown game");
	}
}

} // End of namespace Ultima

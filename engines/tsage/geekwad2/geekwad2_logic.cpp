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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/config-manager.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/geekwad2/geekwad2_logic.h"
#include "tsage/geekwad2/geekwad2_scenes0.h"

namespace TsAGE {

namespace Geekwad2 {

void Geekwad2Game::start() {
	// Start the game
	g_globals->_sceneManager.changeScene(150);
}

Scene *Geekwad2Game::createScene(int sceneNumber) {
	switch (sceneNumber) {
	/* Scene group #0 */
	// Spaced Solicitors
	case 10: return new Scene10();

	// Tsunami title screen
	case 150: return new Scene150();

	// Credits screen
	case 200: return new Scene200();

	default:
		error("Unknown scene number - %d", sceneNumber);
		break;
	}
}

void Geekwad2Game::rightClick() {
}

/**
 * Returns true if it is currently okay to restore a game
 */
bool Geekwad2Game::canLoadGameStateCurrently() {
	return true;
}

/**
 * Returns true if it is currently okay to save the game
 */
bool Geekwad2Game::canSaveGameStateCurrently() {
	return true;
}

void Geekwad2Game::processEvent(Event &event) {
}

/*--------------------------------------------------------------------------*/

SceneExt::SceneExt(): Scene() {
}

} // End of namespace Geekwad2

} // End of namespace TsAGE

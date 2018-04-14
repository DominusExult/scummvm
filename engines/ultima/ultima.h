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

#ifndef ULTIMA_H
#define ULTIMA_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/endian.h"
#include "common/hash-str.h"
#include "common/serializer.h"
#include "common/random.h"
#include "common/savefile.h"
#include "common/util.h"
#include "engines/engine.h"

namespace Ultima {

enum {
	kDebugLevelScript      = 1 << 0
};

enum GameType {
	GType_Ultima1	= 0
};

struct UltimaGameDescription;

class Debugger;
class Events;
class MainGameWindow;
class MouseCursor;
class Resources;
class Game;

namespace Gfx {
	class Screen;
}

class UltimaEngine : public Engine {
private:
	Common::RandomSource _randomSource;
private:
	/**
	 * Initialize the engine
	 */
	bool initialize();

	/**
	 * Deinitialize the engine
	 */
	virtual void deinitialize();
public:
	const UltimaGameDescription *_gameDescription;
	Debugger *_debugger;
	Events *_events;
	MainGameWindow *_window;
	MouseCursor *_mouseCursor;
	Gfx::Screen *_screen;
public:
	UltimaEngine(OSystem *syst, const UltimaGameDescription *gameDesc);
	virtual ~UltimaEngine();

	/**
	 * Main method for running the game
	 */
	virtual Common::Error run();

	/**
	 * Play the game
	 */
	void playGame();

	/**
	 * Returns the Id of the game
	 */
	GameType getGameID() const;

	/**
	 * Return the game's language
	 */
	Common::Language getLanguage() const;

	/**
	 * Return a random number
	 */
	int getRandomNumber(int limit) { return _randomSource.getRandomNumber(limit - 1); }

	/**
	 * Creates a new project hierarchy for the game, that contains all
	 * the logic for playing that particular game.
	 */
	Game *createProject() const;
};

extern UltimaEngine *g_vm;

} // End of namespace Ultima

#endif

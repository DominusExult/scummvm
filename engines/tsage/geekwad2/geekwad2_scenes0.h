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

#ifndef TSAGE_GEEKWAD2_SCENES0_H
#define TSAGE_GEEKWAD2_SCENES0_H

#include "common/scummsys.h"
#include "tsage/converse.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"
#include "tsage/sound.h"
#include "tsage/geekwad2/geekwad2_logic.h"
#include "tsage/geekwad2/geekwad2_speakers.h"

namespace TsAGE {

namespace Geekwad2 {

using namespace TsAGE;

class Scene150: public Scene {
	/* Actions */
	class Action1 : public Action {
	public:
		virtual void signal();
	};
public:
	Action1 _action1;
	ScenePalette _scenePalette;
	SceneObject _object1, _object2, _object3, _object4;
	SceneObject _object5, _object6, _object7, _object8;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
};

class Scene200: public Scene {
	/* Actions */
	class Action1: public Action {
	public:
		virtual void signal();
	};
	class Action2: public Action {
	public:
		virtual void signal();
	};
public:
	Action1 _action1;
	Action2 _action2;
	ASound _sound1, _sound2;
	SceneObject _object1, _object2, _object3, _object4;
	int _field846, _field848;
	int _fieldAB2, _fieldAB4, _creditIndex;
	SynchronizedList<SceneText *> _list1;
	SynchronizedList<SceneObject2 *> _list2;

	Scene200();
	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void process(Event &event);
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif

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

#ifndef ULTIMA_ULTIMA1_GFX_VIEW_CHAR_GEN_H
#define ULTIMA_ULTIMA1_GFX_VIEW_CHAR_GEN_H

#include "ultima/gfx/visual_container.h"
#include "ultima/games/shared/core/character.h"
#include "graphics/managed_surface.h"

namespace Ultima {
namespace Ultima1 {
namespace U1Gfx {

#define ATTRIBUTE_COUNT 6

/**
 * This class implements the character generation view
 */
class ViewCharacterGeneration : public Gfx::VisualContainer {
	DECLARE_MESSAGE_MAP;
	bool KeypressMsg(CKeypressMsg &msg);
	bool ShowMsg(CShowMsg &msg);
	bool HideMsg(CHideMsg &msg);
private:
	enum Flag {
		FLAG_FRAME = 1, FLAG_ATTRIBUTES = 2, FLAG_ATTR_POINTERS = 4, FLAG_HELP = 8, FLAG_RACE = 16,
		FLAG_SEX = 32, FLAG_CLASS = 64,
		FLAG_INITIAL = FLAG_FRAME | FLAG_ATTRIBUTES | FLAG_ATTR_POINTERS | FLAG_HELP
	};
	uint _flags;
	Shared::Character _character;
	int _pointsRemaining;
	int _selectedAttribute;
	uint *_attributes[ATTRIBUTE_COUNT];
private:
	/**
	 * Set state within the view
	 */
	void setMode(Flag flag);

	/**
	 * Draw the outer frame for the view
	 */
	void drawFrame(Gfx::VisualSurface &s);

	/**
	 * Draw the attribute list
	 */
	void drawAttributes(Gfx::VisualSurface &s);

	/**
	 * Draw the pointers to the currently selected attribute
	 */
	void drawAttributePointers(Gfx::VisualSurface &s);

	/**
	 * Draw the help text
	 */
	void drawHelp(Gfx::VisualSurface &s);

	/**
	 * Draw the race selection
	 */
	void drawRace(Gfx::VisualSurface &s);

	/**
	 * Draw the sex selection
	 */
	 void drawSex(Gfx::VisualSurface &s);

	 /**
	  * Draw the class selection
	  */
	 void drawClass(Gfx::VisualSurface &s);
public:
	CLASSDEF;

	/**
	 * Constructor
	 */
	ViewCharacterGeneration(TreeItem *parent = nullptr);
	
	/**
	 * Destructor
	 */
	virtual ~ViewCharacterGeneration() {}

	/**
	 * Draw the game screen
	 */
	virtual void draw();
};

} // End of namespace U1Gfx
} // End of namespace Shared
} // End of namespace Ultima

#endif

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

#ifndef ULTIMA_SAVEABLE_OBJECT_H
#define ULTIMA_SAVEABLE_OBJECT_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/hash-str.h"
#include "common/list.h"
#include "ultima/core/file.h"

namespace Ultima {

class SaveableObject;

class ClassDef;
typedef ClassDef(*ClassDefFn)();

class ClassDef {
private:
	ClassDefFn _parentFn;
public:
	const char *_className;
public:
	ClassDef(const char *className, const ClassDefFn parentFn) :
		_className(className), _parentFn(parentFn) {}

	bool hasParent() const { return _parentFn != nullptr; }
	ClassDef parent() const { return (*_parentFn)(); }
	bool operator==(const ClassDef &right) const {
		return !strcmp(_className, right._className);
	}
};

#define CLASSDEF \
	static ClassDef type(); \
	virtual ClassDef getType() const { return type(); }


class SaveableObject {
public:
	CLASSDEF
	virtual ~SaveableObject() {}

	bool isInstanceOf(const ClassDef &classDef) const;
};

} // End of namespace Ultima

#endif

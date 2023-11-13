//
//  Object.cpp
//  Template Project
//
//  Created by Jean-Yves Hervé on 2023-10-17.
//

#include "Object.h"

using namespace othello;

Object::Object(float x, float y, float angle)
	:	x_(x),
		y_(y),
		angle_(angle)
{}

Object::Object(const TilePoint& tile, float angle)
	:	x_(tile.x),
		y_(tile.y),
		angle_(angle)
{}



//
//  GraphicObject.cpp
//  Template Project
//
//  Created by Jean-Yves Hervé on 2023-10-05.
//

#include <iostream>
#include "GraphicObject.h"

using namespace othello;

GraphicObject::GraphicObject(float x, float y, float angle)
	:	Object(x, y, angle)
{}

GraphicObject::GraphicObject(const TilePoint& tile, float angle)
	:	Object(tile, angle)
{}

void GraphicObject::draw() const {
    
}

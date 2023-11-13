//
//  AnimatedObject.cpp
//  Template Project
//
//  Created by Jean-Yves Hervé on 2023-10-17.
//

#include "Board.h"
#include "AnimatedObject.h"

using namespace othello;

AnimatedObject::AnimatedObject(float x, float y, float angle,
								 float vx, float vy, float spin)
	:	Object(x, y, angle),
		//
		vx_(vx),
		vy_(vy),
		spin_(spin)
{
}

AnimatedObject::AnimatedObject(const TilePoint& tile, float angle, float vx, float vy, float spin)
	:	Object(tile, angle),
		//
		vx_(vx),
		vy_(vy),
		spin_(spin)
{
}


void AnimatedObject::update(float dt) {
    
}

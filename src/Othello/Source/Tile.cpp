//
//  Tile.cpp
//  othello
//
//  Created by Michael Felix on 11/13/23.
//

#include "Tile.hpp"

using namespace othello;

Tile::Tile(TilePoint& loc, float red, float blue, float green, std::shared_ptr<Player> owner)
    :   Object(loc, 0),
        GraphicObject(loc, 0),
        AnimatedObject(loc, 0, 0, 0, 0),
        red_(red),
        blue_(blue),
        green_(green),
        owner_(owner),
        disc_(nullptr) // tiles cannot be initialized with discs on them, discs must be placed afterwards
{
    
}


void Tile::draw() const
{
    //    save the current coordinate system (origin, axes, scale)
    glPushMatrix();
    
    //    move to the center of the disk
    glTranslatef(getX(), getY(), 0.f);
        
    // apply rotation
    glRotatef(getAngle(), 0.f, 0.f, 1.f);
    
    //    apply the radius as a scale
    glScalef(1, 1, 1.f);
    
    
    glColor3f(red_, blue_, green_);
    glBegin(GL_POLYGON);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(+0.5f, -0.5f);
        glVertex2f(+0.5f, +0.5f);
        glVertex2f(-0.5f, +0.5f);
    glEnd();
    
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(+0.5f, -0.5f);
        glVertex2f(+0.5f, +0.5f);
        glVertex2f(-0.5f, +0.5f);
    glEnd();
    
    //    restore the original coordinate system (origin, axes, scale)
    glPopMatrix();
    
}

void Tile::update(float dt) {
    
}

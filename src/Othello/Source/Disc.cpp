//
//  Disc.cpp
//  Othello
//
//  Created by Michael Felix on 11/13/23.
//

#include "Disc.hpp"

using namespace othello;

const int Disc::_numCirPoints = 18;
float** Disc::_circlePoints;

Disc::Disc(TilePoint& loc, RGBColor color)
    :   Object(loc, 0),
        GraphicObject(loc, 0),
        AnimatedObject(loc, 0, 0, 0, 0),
        color_(color),
        size_(0.37)
{
    _circlePoints = new float*[_numCirPoints];
    for (int k=0; k < _numCirPoints; k++) {
        _circlePoints[k] = new float[2];
    }
    float angleStep = 2.f*M_PI/_numCirPoints;
    float theta;
    for (int k = 0; k < _numCirPoints; k++) {
        theta = k * angleStep;
        _circlePoints[k][0] = cosf(theta) * size_;
        _circlePoints[k][1] = sinf(theta) * size_;
    }
}

Disc::Disc(const Disc& obj)
    :   Object(obj.getPos(), 0),
        GraphicObject(obj.getPos(), 0),
        AnimatedObject(obj.getPos(), 0, 0, 0, 0),
        color_(obj.color_),
        size_(0.37)
{
    _circlePoints = new float*[_numCirPoints];
    for (int k=0; k < _numCirPoints; k++) {
        _circlePoints[k] = new float[2];
    }
    float angleStep = 2.f*M_PI/_numCirPoints;
    float theta;
    for (int k = 0; k < _numCirPoints; k++) {
        theta = k * angleStep;
        _circlePoints[k][0] = cosf(theta) * size_;
        _circlePoints[k][1] = sinf(theta) * size_;
    }
}

void Disc::draw() const
{
    //    save the current coordinate system (origin, axes, scale)
    glPushMatrix();
    
    //    move to the center of the disk
    glTranslatef(getX(), getY(), 0.f);
        
    // apply rotation
    glRotatef(getAngle(), 0.f, 0.f, 1.f);
    
    //    apply the radius as a scale
    glScalef(1, 1, 1.f);
    
    glColor3f(color_.red, color_.green, color_.blue);
    
    glBegin(GL_POLYGON);
    for (int i = 0; i < _numCirPoints; i++) {
        glVertex2f(_circlePoints[i][0], _circlePoints[i][1]);
    }
    glEnd();

    //    restore the original coordinate system (origin, axes, scale)
    glPopMatrix();
    
}

void Disc::update(float dt) {
    
}

//
//  Tile.hpp
//  othello
//
//  Created by Michael Felix on 11/13/23.
//

#ifndef Tile_hpp
#define Tile_hpp

#include "GraphicObject.h"
#include "AnimatedObject.h"

using namespace othello;

class Tile: public GraphicObject, public AnimatedObject {
private:
    
    float red_, blue_, green_; // false = white, true = black
    
public:
    Tile(TilePoint& loc, float red, float blue, float green);
    
    void draw() const;
    void update(float dt);
    
    inline int getRow() {
        return (int)getX();
    }
    inline int getCol() {
        return (int)getY();
    }
    inline TilePoint getPos() {
        return TilePoint{getRow(), getCol()};
    }
    inline void setColor(RGBColor color) {
        red_ = color.red;
        green_ = color.green;
        blue_ = color.blue;
    }
    inline void setColor(float red, float green, float blue) {
        red_ = red;
        green_ = green;
        blue_ = blue;
    }
};

#endif /* Tile_hpp */

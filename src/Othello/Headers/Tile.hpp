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
   
    bool color_; // false = white, true = black
    
public:
    Tile(TilePoint& loc, float color);
    
    void draw() const;
    void update(float dt);
};

#endif /* Tile_hpp */

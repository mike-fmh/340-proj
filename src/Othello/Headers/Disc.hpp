//
//  Disc.hpp
//  Othello
//
//  Created by Michael Felix on 11/13/23.
//

#ifndef Disc_hpp
#define Disc_hpp

#include "GraphicObject.h"
#include "AnimatedObject.h"

namespace othello {
    class Disc: public GraphicObject, public AnimatedObject {
        private:
            static float** _circlePoints;
            static const int _numCirPoints;
            bool color_; // false = white, true = black
            float size_; // how large should the disc be?
        
        public:
            Disc(TilePoint& loc, float color);
            
            void draw() const;
            void update(float dt);
    };
}

#endif /* Disc_hpp */

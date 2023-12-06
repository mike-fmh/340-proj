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
            /// Stores the points relative to the disc's center that constitute its circular shape.
            static float** _circlePoints;
        
            /// The number of points used to draw the disc (the circle).
            static const unsigned int _numCirPoints;
        
            /// Color of the disc.
            RGBColor color_;
        
            /// Scale of the disc.
            float size_;
        
            /// Variables used to time the disc's animation color change after being flipped.
            float switchColorAfter_, curSwitchTimer_;
            RGBColor colorToSwitchTo_;
        
        public:
            /// Creates a new Game Piece (Disc).
            /// @param loc TilePoint location on the game board of the new disc.
            /// @param color The color of the disc.
            Disc(TilePoint& loc, RGBColor color);
        
            /// The draw function is called every frame,
            void draw() const;
        
            /// The update function is called multiple times between every frame.
            /// @param dt Time since the last update function ran.
            void update(float dt);
        
            /// Getters for the disc's coords.
            inline int getRow() const {
                return (int)getX();
            }
            inline int getCol() const {
                return (int)getY();
            }
            inline TilePoint getPos() const {
                return TilePoint{getRow(), getCol()};
            }
        
            /// Getter for the disc's color
            inline RGBColor getColor() {
                return color_;
            }
        
            /// Setters for the disc's color
            inline void setColor(RGBColor color) {
                color_ = color;
            }
            inline void setColor(float red, float green, float blue) {
                color_ = RGBColor{red, green, blue};
            }
        
            /// Sets the disc's color to the given one after a desired amount of seconds.
            inline void setColorAfter(RGBColor color, float secs) {
                colorToSwitchTo_ = color;
                switchColorAfter_ = secs;
            }
    };
}

#endif /* Disc_hpp */

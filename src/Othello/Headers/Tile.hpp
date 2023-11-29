//
//  Tile.hpp
//  othello
//
//  Created by Michael Felix on 11/13/23.
//

#ifndef Tile_hpp
#define Tile_hpp

#include <memory>
#include "Player.hpp"
#include "GraphicObject.h"
#include "AnimatedObject.h"

namespace othello {
    class Tile: public GraphicObject, public AnimatedObject {
        private:
            /// if this tile has a disc on it, which player controls this disc?
            std::shared_ptr<Player> owner_;
            
            /// reference to the disc (piece) that's been placed on this tile
            std::shared_ptr<Disc> disc_;
        
            float red_, blue_, green_; // false = white, true = black
            
        public:
            Tile(TilePoint& loc, float red, float blue, float green, std::shared_ptr<Player> owner);
            Tile(const Tile& obj); // copy;
        
            void draw() const;
            void update(float dt);
            
            inline void setOwner(std::shared_ptr<Player>& player) {
                owner_ = player;
            }
        
            inline void setOwner(Player& player) {
                owner_ = std::make_shared<Player>(player);
            }
        
            /// Returns the owner (player) that owns the disc placed at this board tile. If no pieces are placed here, return nullplayer
            inline std::shared_ptr<Player> getPieceOwner() {
                return owner_;
            }
            
            inline std::shared_ptr<Disc> getPiece() {
                return disc_;
            }
        
            inline void setPiece(std::shared_ptr<Disc>& piece) {
                disc_ = piece;
            }
        
            /// Returns if this tile is equal to another tile reference
            /// @param other reference to the other tile object to check equality with
            inline bool posIsEqual(std::shared_ptr<Tile>& other) {
                return ((other->getRow() == getRow()) && (other->getCol() == getCol()));
            }
            inline int getRow() const {
                return (int)getX();
            }
            inline int getCol() const {
                return (int)getY();
            }
            inline TilePoint getPos() const {
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
}

#endif /* Tile_hpp */

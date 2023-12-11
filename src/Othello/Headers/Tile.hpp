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
            /// If this tile has a disc on it, which player controls this disc?
            std::shared_ptr<Player> owner_;
            
            /// Reference to the disc (piece) that's been placed on this tile.
            std::shared_ptr<Disc> disc_;
        
            /// Values representing the actual color of the tile as rendered in the game window.
            float red_, blue_, green_;
            
        public:
            /// Create a new Tile at a TilePoint location.
            /// @param loc Location where the tile should be located on the game board.
            /// @param red Red value for the tile's color.
            /// @param blue Blue value for the tile's color.
            /// @param green Green value for the tile's color.
            /// @param owner Reference to the player that controls the disc placed on this tile (null player if the tile is blank).
            Tile(TilePoint& loc, float red, float blue, float green, std::shared_ptr<Player> owner);
        
            /// The draw function is called every frame,
            void draw() const;
        
            /// The update function is called multiple times between every frame.
            /// @param dt Time since the last update function ran.
            void update(float dt);
            
            /// Set the owner of the Tile (the player who controls the Disc that's placed on it).
            /// @param player Reference to the player who owns the tile's disc.
            inline void setOwner(std::shared_ptr<Player>& player) {
                owner_ = player;
            }
        
            /// Returns the owner (player) that owns the disc placed at this board tile. If no pieces are placed here, return nullplayer
            inline std::shared_ptr<Player> getPieceOwner() {
                return owner_;
            }
            
            /// Return a reference to the Disc placed on this Tile.
            inline std::shared_ptr<Disc> getPiece() {
                return disc_;
            }
        
            /// Sets the piece placed on this tile to the given Disc reference.
            /// @param piece The reference to the Disc that should be placed on this Tile.
            inline void setPiece(std::shared_ptr<Disc>& piece) {
                disc_ = piece;
            }
        
            /// Returns if this tile is equal to another tile reference
            /// @param other reference to the other tile object to check equality with
            inline bool posIsEqual(std::shared_ptr<Tile>& other) {
                return ((other->getRow() == getRow()) && (other->getCol() == getCol()));
            }
        
            /// Returns the row on the game board where this Tile is located.
            inline int getRow() const {
                return (int)getX();
            }
        
            /// Returns the column on the game board where this Tile is located.
            inline int getCol() const {
                return (int)getY();
            }
        
            /// Returns the TilePoint location on the game board where thiis Tile is located.
            inline TilePoint getPos() const {
                return TilePoint{getRow(), getCol()};
            }
        
            /// Set the Tile's color as rendered in the game window (does not interact with the tile's Disc color).
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

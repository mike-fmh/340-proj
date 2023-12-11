//
//  Player.hpp
//  Othello
//
//  Created by Michael Felix on 11/16/23.
//

#ifndef Player_hpp
#define Player_hpp

#include <vector>
#include <memory>
#include "Disc.hpp"

namespace othello {
    class Player {
    private:
        /// Color of this player's discs (pieces)
        const RGBColor DiscColor_;

    public:
        Player(RGBColor myColor);
        
        //disabled constructors & operators
        Player(Player&& obj) = delete;        // move
        Player(const Player& obj) = delete;
        Player& operator = (const Player& obj) = delete;    // copy operator
        Player& operator = (Player&& obj) = delete;        // move operator
        
        /// Return the color of the pieces that the player owns
        inline RGBColor getMyColor() {
            return DiscColor_;
        }
    };
}

#endif /* Player_hpp */

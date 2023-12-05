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
#include <string>


namespace othello {
    class Player {
    private:
        /// color of this player's discs (pieces)
        const RGBColor DiscColor_;
        const std::string name_;

    public:
        
        Player(RGBColor myColor);
        Player(RGBColor myColor, std::string name);
        
        //disabled constructors & operators
        Player(Player&& obj) = delete;        // move
        Player(const Player& obj) = delete;
        Player& operator = (const Player& obj) = delete;    // copy operator
        Player& operator = (Player&& obj) = delete;        // move operator
        
        inline RGBColor getMyColor() {
            return DiscColor_;
        }
        
        inline std::string getName() {
            return name_;
        }
    };
}

#endif /* Player_hpp */

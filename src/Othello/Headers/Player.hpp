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
        
        /// all pieces under this player's control
        std::vector<std::shared_ptr<Disc>> myPieces_;
    public:
        
        Player(RGBColor myColor);
        Player(RGBColor myColor, std::string name);
        Player(const Player& obj);    // copy
        Player(std::shared_ptr<Player> obj);    // shared_ptr copy
        
        //disabled constructors & operators
        Player(Player&& obj) = delete;        // move
        Player& operator = (const Player& obj) = delete;    // copy operator
        Player& operator = (Player&& obj) = delete;        // move operator
        
        /*
        /// remove pieces from this player's control
        /// @param pieces the vector of pieces to remove from my control
        void rmPieces(std::vector<std::shared_ptr<Disc>> pieces);
        */
        
        inline RGBColor getMyColor() {
            return DiscColor_;
        }
        
        inline std::string getName() {
            return name_;
        }
        
        /// returns a reference to a vector of all pieces under my control
        inline std::vector<std::shared_ptr<Disc>>* getMyPieces() {
            return &myPieces_;
        }
        
        inline void addPiece(std::shared_ptr<Disc> piece) {
            myPieces_.push_back(piece);
        }
    };
}

#endif /* Player_hpp */

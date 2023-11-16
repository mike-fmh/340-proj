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
        /// color of this player's discs (pieces)
        const RGBColor DiscColor_;
        
        /// all pieces under this player's control
        std::vector<std::shared_ptr<Disc>> myPieces_;
    public:
        
        Player(RGBColor myColor);
        
        //disabled constructors & operators
        Player() = delete;
        Player(const Player& obj) = delete;    // copy
        Player(Player&& obj) = delete;        // move
        Player& operator = (const Player& obj) = delete;    // copy operator
        Player& operator = (Player&& obj) = delete;        // move operator
        
        /// remove pieces from this player's control
        /// @param pieces the vector of pieces to remove from my control
      //  void rmPieces(std::vector<std::shared_ptr<Disc>> pieces);
        
        inline RGBColor getMyColor() {
            return DiscColor_;
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

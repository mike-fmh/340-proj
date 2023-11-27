//
//  AiPlayer.hpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#ifndef AiPlayer_hpp
#define AiPlayer_hpp

#include "Player.hpp"
#include "Tile.hpp"


namespace othello {

/// The score the AI creates for each possible move based on the boardstate after the move
struct MoveScore {
    int mobility;
    int stability;
    int cornerControl;
    int totalScore;
};

class AiPlayer : public Player {
private:
    
public:
    AiPlayer(RGBColor myColor);
    AiPlayer(RGBColor myColor, std::string name);
        
    //disabled constructors & operators
    AiPlayer() = delete;
    AiPlayer(const AiPlayer& obj) = delete;    // copy
    AiPlayer(AiPlayer&& obj) = delete;        // move
    AiPlayer& operator = (const AiPlayer& obj) = delete;    // copy operator
    AiPlayer& operator = (AiPlayer&& obj) = delete;        // move operator
};

}

#endif /* AiPlayer_hpp */

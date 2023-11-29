//
//  AiPlayer.cpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#include "AiPlayer.hpp"
#include <iostream>

using namespace std;
using namespace othello;

// weights for each factor based on their importance
const int AiPlayer::MOBILITY_WEIGHT_ = 1;
const int AiPlayer::STABILITY_WEIGHT_ = 2;
const int AiPlayer::CORNER_WEIGHT_ = 3;

AiPlayer::AiPlayer(RGBColor color)
    :   Player(color)
{
    
}

AiPlayer::AiPlayer(RGBColor color, std::string name)
    :   Player(color, name)
{
    
}

AiPlayer::AiPlayer(std::shared_ptr<Player> basePlayer)
    :   Player(basePlayer->getMyColor(), basePlayer->getName())
{

}

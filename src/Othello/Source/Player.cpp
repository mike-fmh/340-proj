//
//  Player.cpp
//  Othello
//
//  Created by Michael Felix on 11/16/23.
//

#include "Player.hpp"

using namespace std;
using namespace othello;

Player::Player(RGBColor color)
    :   DiscColor_(color),
        myPieces_(vector<shared_ptr<Disc>>()),
        name_("none")
{
    
}


Player::Player(RGBColor color, std::string name)
    :   DiscColor_(color),
        myPieces_(vector<shared_ptr<Disc>>()),
        name_(name)
{
    
}


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

Player::Player(const Player& obj)
    :   DiscColor_(obj.DiscColor_),
        myPieces_(vector<shared_ptr<Disc>>()),
        name_(obj.name_)
{
    
}


Player::Player(std::shared_ptr<Player> obj)
    :   DiscColor_(obj->DiscColor_),
        myPieces_(vector<shared_ptr<Disc>>()),
        name_(obj->name_)
{
    
}

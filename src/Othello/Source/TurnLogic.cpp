//
//  TurnLogic.cpp
//  Othello
//
//  Created by Michael Felix on 11/16/23.
//

#include "TurnLogic.hpp"

using namespace std;
using namespace othello;


// othello will always have 2 players
const int TurnLogic::NUM_GAME_PLAYERS = 2;


TurnLogic::TurnLogic(shared_ptr<Player> playerWhite, shared_ptr<Player> playerBlack, shared_ptr<vector<vector<shared_ptr<Tile>>>> board)
    :   startingPlayer_(playerWhite), // white always starts in othello
        currentPlayerTurn_(playerWhite),
        boardTiles_(board)
{
    
}

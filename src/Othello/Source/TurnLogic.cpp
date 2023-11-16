//
//  TurnLogic.cpp
//  Othello
//
//  Created by Michael Felix on 11/16/23.
//

#include "TurnLogic.hpp"

using namespace std;
using namespace othello;


TurnLogic::TurnLogic(int numPlayers, int startingPlayer, shared_ptr<vector<vector<shared_ptr<Tile>>>> board)
    :   NUM_GAME_PLAYERS(numPlayers),
        startingPlayer_(startingPlayer),
        currentPlayerTurn_(startingPlayer),
        boardTiles_(board)
{
    
}

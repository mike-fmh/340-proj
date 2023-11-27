//
//  AiPlayer.cpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#include "AiPlayer.hpp"

using namespace std;
using namespace othello;

// weights for each factor based on their importance
const int AiPlayer::MOBILITY_WEIGHT_ = 1;
const int AiPlayer::STABILITY_WEIGHT_ = 2;
const int AiPlayer::CORNER_WEIGHT_ = 3;

AiPlayer::AiPlayer(RGBColor color)
    :   Player(color),
        hypotheticalMoveState_(nullptr)
{
    
}

AiPlayer::AiPlayer(RGBColor color, std::string name)
    :   Player(color, name),
        hypotheticalMoveState_(nullptr)
{
    
}

void AiPlayer::evalGamestateScore(shared_ptr<TurnLogic> currentGamestate) {
    int mobility, stability, cornerPieces;
    GamestateScore curScore;
    
    // tiles where I currently have pieces placed
    std::vector<std::shared_ptr<Tile>> allMyPieces;
    
    // tiles where I can place pieces
    std::vector<std::shared_ptr<Tile>> possibleMoveTiles;
    
    // populate possible moves
    currentGamestate->getPlayableTiles(*this, possibleMoveTiles);
    
    mobility = (int)possibleMoveTiles.size();
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    
    stability = 0;
    
    
    
}

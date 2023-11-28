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

GamestateScore AiPlayer::evalGamestateScore(shared_ptr<TurnLogic> currentGamestate) {
    int mobility, stability, cornerPieces;
    GamestateScore curScore;
    
    // tiles where I currently have pieces placed
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;
    // tiles where I can place pieces
    std::vector<std::shared_ptr<Tile>> possibleMoveTiles;
    
    /// Calculate stability and count corner pieces
    currentGamestate->getPlayerTiles(*this, allMyPieces); // populate my tiles
    cornerPieces = 0;
    stability = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (currentGamestate->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (currentGamestate->discIsStable(thisTile, *this)) // if the tile isn't flankable by the opponent
                stability++;
        }
    }
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT_;
    curScore.stabilityScore = stability * STABILITY_WEIGHT_;
    
    /// Find mobility (number of possible moves)
    currentGamestate->getPlayableTiles(*this, possibleMoveTiles); // populate possible moves
    
    mobility = (int)possibleMoveTiles.size();
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    
    return curScore;
}

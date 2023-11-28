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
    :   Player(color)
{
    
}

AiPlayer::AiPlayer(RGBColor color, std::string name)
    :   Player(color, name)
{
    
}

TilePoint AiPlayer::findBestMove(shared_ptr<GameState>& currentGamestate) {
    TilePoint bestMove;
    
    vector<shared_ptr<Tile>> possibleMoves;
    currentGamestate->getPlayableTiles(*this, possibleMoves);
    
    for (shared_ptr<Tile> thisMove : possibleMoves) {
        GameState hypotheticalGamestate = GameState(*currentGamestate);
    }
    
    return bestMove;
}

GamestateScore AiPlayer::evalGamestateScore(shared_ptr<GameState>& currentGamestate) {
    int mobility, pseudostability, stability, cornerPieces;
    GamestateScore curScore;
    
    /// Find mobility (number of possible moves)
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    currentGamestate->getPlayableTiles(*this, possibleMoves);
    mobility = (int)possibleMoves.size();
    
    /// Calculate stability and count corner pieces
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;  // tiles where I currently have pieces placed
    currentGamestate->getPlayerTiles(*this, allMyPieces); // populate my tiles
    cornerPieces = 0;
    pseudostability = 0;
    stability = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (currentGamestate->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (currentGamestate->discIsPseudostable(thisTile, *this)) // if the tile isn't flankable by the opponent
                pseudostability++;
        }
    }
    
    /// Multiply by weights and sum products together
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT_;
    curScore.pseudostabilityScore = pseudostability * STABILITY_WEIGHT_;
    curScore.stabilityScore = stability * STABILITY_WEIGHT_;
    curScore.totalScore = curScore.sum();
    return curScore; // totalScore represents the overall positional score for the AI for currentGamestate
}

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


TilePoint AiPlayer::findBestMove(shared_ptr<GameState>& currentGamestate, vector<shared_ptr<Tile>> possibleMoves) {
    TilePoint bestMove;
    unsigned int highestMovescore, curMovescore;
    highestMovescore = 0;

    for (auto tile : possibleMoves) {
        cout << "possible move: " << tile->getRow() << ", " << tile->getCol() << endl;
    }
    cout << endl;
    
    for (shared_ptr<Tile> thisMove : possibleMoves) {
        shared_ptr<GameState> hypotheticalGamestate = make_shared<GameState>(*currentGamestate);
        hypotheticalGamestate->placePiece(*this, thisMove);
        curMovescore = evalGamestateScore(hypotheticalGamestate);
        if (curMovescore > highestMovescore) {
            highestMovescore = curMovescore;
            bestMove = thisMove->getPos();
        }
    }
    cout << bestMove.x << ", " << bestMove.y << endl;
    return bestMove;
}

unsigned int AiPlayer::evalGamestateScore(shared_ptr<GameState>& hypotheticalGamestate) {
    int mobility, pseudostability, stability, cornerPieces;
    GamestateScore curScore;
    
    /// Find mobility (number of possible moves)
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    hypotheticalGamestate->getPlayableTiles(*this, possibleMoves);
    mobility = (int)possibleMoves.size();
    
    /// Calculate stability and count corner pieces
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;  // tiles where I currently have pieces placed
    hypotheticalGamestate->getPlayerTiles(*this, allMyPieces); // populate my tiles
    cornerPieces = 0;
    pseudostability = 0;
    stability = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (hypotheticalGamestate->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (hypotheticalGamestate->discIsPseudostable(thisTile, *this)) // if the tile isn't flankable by the opponent
                pseudostability++;
        }
    }
    
    /// Multiply by weights and sum products together
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT_;
    curScore.pseudostabilityScore = pseudostability * STABILITY_WEIGHT_;
    curScore.stabilityScore = stability * STABILITY_WEIGHT_;
    curScore.totalScore = curScore.sum();
    return curScore.totalScore; // totalScore represents the overall positional score for the AI for currentGamestate
}

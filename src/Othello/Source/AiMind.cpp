//
//  AiMind.cpp
//  Othello
//
//  Created by Michael Felix on 12/5/23.
//

#include "AiMind.hpp"

using namespace std;
using namespace othello;

RGBColor AiMind::WHITE = RGBColor{1, 1, 1};
RGBColor AiMind::BLACK = RGBColor{0, 0, 0};


AiMind::AiMind(unsigned int mobilityWeight, unsigned int stabilityWeight, unsigned int cornerWeight, unsigned int powerWeight, RGBColor defaultTileCol)
    :
    MOBILITY_WEIGHT_(mobilityWeight),
    STABILITY_WEIGHT_(stabilityWeight),
    CORNER_WEIGHT_(cornerWeight),
    POWER_WEIGHT_(powerWeight),
    DEFAULT_TILE_COLOR_(defaultTileCol)
{
    
}


unsigned int AiMind::minimax(bool maximizing, unsigned int depth, shared_ptr<Player>& playerBlack, shared_ptr<Player>& playerWhite, shared_ptr<Board>& mainGameBoard, shared_ptr<GameState>& layout) {
    if (depth == 0) // base case
        return evalGamestateScore(playerBlack, layout);
    
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    
    if (maximizing) {
        // simulate black placing a piece that puts them at the largest advantage
        layout->getPlayableTiles(playerBlack, possibleMoves);
        unsigned int maxMove = 0;
        unsigned int maxMoveInd = 0;
        for (unsigned int i = 0; i < possibleMoves.size(); i++) {
            shared_ptr<Tile> thisMove = possibleMoves[i];
            
            shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
            shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
            shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
            shared_ptr<Board> tempBoard = make_shared<Board>(DEFAULT_TILE_COLOR_, tempNull);
            shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
            shared_ptr<Player> tempOwner;
            for (shared_ptr<Disc> piece : mainGameBoard->getAllPieces()) {
                
                TilePoint thisPiecePos = piece->getPos();
                if (piece->getColor().isEqualTo(WHITE)) {
                    tempOwner = tempWhite;
                } else {
                    tempOwner = tempBlack;
                }
                tempGamestate->addGamePiece(thisPiecePos, tempOwner);
            }
            TilePoint thisMoveLoc = thisMove->getPos();
            shared_ptr<Tile> hypMove = tempBoard->getBoardTile(thisMoveLoc);
            tempGamestate->placePiece(tempBlack, hypMove, true);
            
            unsigned int thisMoveScore = minimax(false, depth - 1, tempBlack, tempWhite, mainGameBoard, tempGamestate);
            if (thisMoveScore > maxMove) {
                maxMove = thisMoveScore;
                maxMoveInd = i;
            }
        }
        return maxMoveInd;
    } else {
        // simulate white placing the piece which puts black at the largest disadvantage
        layout->getPlayableTiles(playerWhite, possibleMoves);
        unsigned int minMove = INT_MAX;
        unsigned int minMoveInd = 0;
        for (unsigned int i = 0; i < possibleMoves.size(); i++) {
            shared_ptr<Tile> thisMove = possibleMoves[i];
            
            shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
            shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
            shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
            shared_ptr<Board> tempBoard = make_shared<Board>(DEFAULT_TILE_COLOR_, tempNull);
            shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
            shared_ptr<Player> tempOwner;
            for (shared_ptr<Disc> piece : mainGameBoard->getAllPieces()) {
                TilePoint thisPiecePos = piece->getPos();
                if (piece->getColor().isEqualTo(WHITE)) {
                    tempOwner = tempWhite;
                } else {
                    tempOwner = tempBlack;
                }
                tempGamestate->addGamePiece(thisPiecePos, tempOwner);
            }
            TilePoint thisMoveLoc = thisMove->getPos();
            shared_ptr<Tile> hypMove = tempBoard->getBoardTile(thisMoveLoc);
            tempGamestate->placePiece(tempWhite, hypMove, true);
            
            unsigned int thisMoveScore = minimax(true, depth - 1, tempBlack, tempWhite, mainGameBoard, tempGamestate);
            if (thisMoveScore < minMove) {
                minMove = thisMoveScore;
                minMoveInd = i;
            }
        }
        return minMoveInd;
    }
}

unsigned int AiMind::evalGamestateScore(shared_ptr<Player>& forWho, shared_ptr<GameState>& layout) {
    int mobility, stability, cornerPieces;
    GamestateScore curScore;
    
    /// Find mobility (number of possible moves)
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    layout->getPlayableTiles(forWho, possibleMoves);
    mobility = (int)possibleMoves.size();
    
    /// Calculate stability and count corner pieces
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;  // tiles where I currently have pieces placed
    layout->getPlayerTiles(forWho, allMyPieces); // populate my tiles
    cornerPieces = 0;
    stability = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (layout->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (layout->discIsStable(thisTile, forWho)) // if the tile isn't flankable by the opponent
                stability++;
        }
    }
    
    /// Multiply by weights and sum products together
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT_;
    curScore.stabilityScore = stability * STABILITY_WEIGHT_;
    curScore.totalScore = curScore.sum();
    
    return curScore.totalScore; // totalScore represents the overall positional score for the AI for currentGamestate
}


unsigned int AiMind::bestMoveHeuristic(shared_ptr<Player>& forWho, shared_ptr<Board>& mainGameBoard, vector<shared_ptr<Tile>>& possibleMoves) {
    unsigned int bestMoveInd = 0;
    unsigned int bestMoveScore = 0;
    unsigned int curMoveScore = 0;
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        shared_ptr<Tile> thisMove = possibleMoves[i];
        unsigned int numFlipped;
        pair<shared_ptr<Player>, shared_ptr<GameState>> thisMoveResult;
        thisMoveResult = applyMove_(forWho, mainGameBoard, thisMove, &numFlipped);
        shared_ptr<Player> newMovePieceOwner = thisMoveResult.first;
        shared_ptr<GameState> newMoveState = thisMoveResult.second;
        curMoveScore = evalGamestateScore(newMovePieceOwner, newMoveState);
        
        if (curMoveScore > bestMoveScore) {
            bestMoveInd = i;
            bestMoveScore = curMoveScore;
        }
    
    }
    return bestMoveInd;
}


pair<shared_ptr<Player>, shared_ptr<GameState>> AiMind::applyMove_(shared_ptr<Player>& forWho, shared_ptr<Board>& mainGameBoard, shared_ptr<Tile>& thisMove, unsigned int* numFlipped) {
    shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
    shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
    shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
    
    shared_ptr<Board> tempBoard = make_shared<Board>(DEFAULT_TILE_COLOR_, tempNull);
    shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
    
    shared_ptr<Player> tempOwner;
    for (shared_ptr<Disc> piece : mainGameBoard->getAllPieces()) {
        
        TilePoint thisPiecePos = piece->getPos();
        if (piece->getColor().isEqualTo(WHITE)) {
            tempOwner = tempWhite;
        } else {
            tempOwner = tempBlack;
        }
        tempGamestate->addGamePiece(thisPiecePos, tempOwner);
    }
    
    TilePoint thisMoveLoc = thisMove->getPos();
    shared_ptr<Tile> hypMove = tempBoard->getBoardTile(thisMoveLoc);
    
    // get AIplayer reference for the temp boardstate
    if (forWho->getMyColor().isEqualTo(WHITE)) {
        tempOwner = tempWhite;
    } else {
        tempOwner = tempBlack;
    }
    *numFlipped = tempGamestate->placePiece(tempOwner, hypMove, true);
    
    return make_pair(tempOwner, tempGamestate);
}

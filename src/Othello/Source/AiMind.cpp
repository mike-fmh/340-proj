//
//  AiMind.cpp
//  Othello
//
//  Created by Michael Felix on 12/5/23.
//

#include "AiMind.hpp"
#include <iostream>


using namespace std;
using namespace othello;

RGBColor AiMind::WHITE = RGBColor{1, 1, 1};
RGBColor AiMind::BLACK = RGBColor{0, 0, 0};


AiMind::AiMind(unsigned int discWeight, unsigned int mobilityWeight, unsigned int stabilityWeight, unsigned int cornerWeight, int cornerAdjWeight, int frontierWeight, RGBColor defaultTileCol)
    :
    MOBILITY_WEIGHT_(mobilityWeight),
    STABILITY_WEIGHT_(stabilityWeight),
    CORNER_WEIGHT_(cornerWeight),
    CORNER_ADJ_WEIGHT_(cornerAdjWeight),
    NUM_FRONTIER_WEIGHT_(frontierWeight),
    NUM_DISC_WEIGHT_(discWeight),
    DEFAULT_TILE_COLOR_(defaultTileCol)
{
    
}


int AiMind::minimax(bool maximizing, unsigned int depth, shared_ptr<Player>& playerBlack, shared_ptr<Player>& playerWhite, shared_ptr<Board>& thisBoard, shared_ptr<GameState>& layout, int alpha, int beta) {
    if (depth == 0) //or game is over // base case
        return evalGamestateScore(playerBlack, layout);
    
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    
    if (maximizing) {
        // simulate black placing a piece that puts them at the largest advantage
        layout->getPlayableTiles(playerBlack, possibleMoves);
        if (possibleMoves.size() == 0) { // no more moves for black
            std::cout << "no more moves in this branch(black)\n";
            return evalGamestateScore(playerBlack, layout);
        }
        int maxEval = INT_MIN;
        for (unsigned int i = 0; i < possibleMoves.size(); i++) {
            shared_ptr<Tile> thisMove = possibleMoves[i];
            int eval = applyMinimaxMove_(maximizing, depth, thisMove, thisBoard, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // alpha-beta pruning
            }
        }
        return maxEval;
    } else {
        // simulate white placing the piece which puts black at the largest disadvantage
        layout->getPlayableTiles(playerWhite, possibleMoves);
        if (possibleMoves.size() == 0) { // no more moves for white
            std::cout << "no more moves in this branch(white)\n";
            return evalGamestateScore(playerBlack, layout);
        }
        int minEval = INT_MAX;
        for (unsigned int i = 0; i < possibleMoves.size(); i++) {
            shared_ptr<Tile> thisMove = possibleMoves[i];
            int eval = applyMinimaxMove_(maximizing, depth, thisMove, thisBoard, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // alpha-beta pruning
            }
        }
        return minEval;
    }
}

int AiMind::applyMinimaxMove_(bool maxing, unsigned int depth, shared_ptr<Tile>& thisMove, shared_ptr<Board>& oldBoard, int alpha, int beta) {
    // each hypothetical move needs a new board object, thus also needs a new gamestate obj and new player objs
    shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
    shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
    shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
    shared_ptr<Board> tempBoard = make_shared<Board>(DEFAULT_TILE_COLOR_, tempNull);
    shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
    shared_ptr<Player> tempOwner;
    // bring the new board up to date
    // oldboard = the board before this hypothetical move
    for (shared_ptr<Disc> piece : oldBoard->getAllPieces()) {
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
    
    // the maximizing player (AI) is black
    if (maxing) {
        tempOwner = tempBlack;
    } else {
        tempOwner = tempWhite;
    }
    tempGamestate->placePiece(tempOwner, hypMove);
    
    return minimax(!maxing, depth - 1, tempBlack, tempWhite, tempBoard, tempGamestate, alpha, beta);
}


int AiMind::evalGamestateScore(shared_ptr<Player>& forWho, shared_ptr<GameState>& layout) {
    int mobility, stability, cornerPieces, cornerAdj, frontiers;
    GamestateScore curScore;
    
    std::vector<std::vector<std::shared_ptr<Tile>>> myTiles;
    
    /// Find mobility (number of possible moves)
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    layout->getPlayableTiles(forWho, possibleMoves);
    mobility = (int)possibleMoves.size();
    
    /// Calculate stability and count corner pieces
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;  // tiles where I currently have pieces placed
    layout->getPlayerTiles(forWho, allMyPieces); // populate my tiles
    cornerPieces = 0;
    cornerAdj = 0;
    stability = 0;
    frontiers = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (layout->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (layout->isCornerAdj(thisTile))
                cornerAdj++;
            if (layout->discIsStable(thisTile, forWho)) // if the tile isn't flankable by the opponent
                stability++;
            frontiers = layout->numFrontierTiles(thisTile);
            layout->getPlayerTiles(forWho, myTiles);
        }
    }
    
    /// Multiply by weights and sum products together
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT_;
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT_;
    curScore.stabilityScore = stability * STABILITY_WEIGHT_;
    curScore.cornerAdjScore = cornerAdj * CORNER_ADJ_WEIGHT_;
    curScore.frontierScore = frontiers * NUM_FRONTIER_WEIGHT_;
    curScore.discScore = (int)myTiles.size() * NUM_DISC_WEIGHT_;
    curScore.totalScore = curScore.sum();
    
    return curScore.totalScore; // totalScore represents the overall positional score for the AI for currentGamestate
}


unsigned int AiMind::bestMoveMinimax(shared_ptr<Player>& playerBlack, shared_ptr<Player>& playerWhite, shared_ptr<Board>& mainGameBoard, shared_ptr<GameState>& mainGameState, vector<shared_ptr<Tile>>& possibleMoves, unsigned int depth) {
    unsigned int bestMoveInd = 0;
    int bestMoveScore = 0;
    int curMoveScore = 0;
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        shared_ptr<Tile> thisMove = possibleMoves[i];
        
        shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
        shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
        shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
        shared_ptr<Board> tempBoard = make_shared<Board>(DEFAULT_TILE_COLOR_, tempNull);
        shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
        shared_ptr<Player> tempOwner;
        // bring the new board up to date
        // thisboard = the board before this hypothetical move
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
        tempGamestate->placePiece(tempBlack, hypMove);
        
        // applying minimax to this hypothetical move will give us the overall score for this move
        curMoveScore = minimax(false, depth, tempBlack, tempWhite, tempBoard, tempGamestate, INT_MIN, INT_MAX);
        std::cout << "cur (" << thisMoveLoc.x << ", " << thisMoveLoc.y << "), "<< i <<": " << curMoveScore << ", num blank adjs: " << tempGamestate->numFrontierTiles(thisMove) << std::endl;
        if (curMoveScore > bestMoveScore) {
            bestMoveInd = i;
            bestMoveScore = curMoveScore;
        }
    
    }
    return bestMoveInd;
}

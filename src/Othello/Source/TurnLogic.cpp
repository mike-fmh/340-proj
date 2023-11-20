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


TurnLogic::TurnLogic(shared_ptr<Player> playerWhite, shared_ptr<Player> playerBlack, shared_ptr<Board> board)
    :   startingPlayer_(playerWhite), // white always starts in othello
        currentPlayerTurn_(playerWhite),
        board_(board),
        boardTiles_(board->getTiles())
{
    
}

bool TurnLogic::TileIsFlanked(std::shared_ptr<Tile> tile) {
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    TilePoint thisPos = tile->getPos();
    std::shared_ptr<Player> player = tile->getPieceOwner();
    
    for (auto dir : nextDir) {
        TilePoint thisDir = TilePoint{thisPos.x + dir.x, thisPos.y + dir.y};
        
        bool foundOpp = false;
        
        while (board_->isValidPosition(thisDir)) {
            auto nextTile = board_->getBoardTile(thisDir);
            if (nextTile->getPieceOwner() == board_->getNullPlayer()) {
                break;
            }

            if (nextTile->getPieceOwner() != player) {
                foundOpp = true;
            } else if (foundOpp) {
                // Found a line of opponent tiles ending with a player tile
                return true;
            } else {
                break;
            }

            thisDir = TilePoint{thisDir.x + dir.x, thisDir.y + dir.y};
        }
    }
    
    return false;
}



void TurnLogic::getPlayableTiles(std::shared_ptr<Player> forWho, std::vector<std::shared_ptr<Tile>>& movableTiles) {
    for (int c = 0; c < boardTiles_->size(); c++) {
        for (int r = 0; r < boardTiles_->at(c).size(); r++) {
            std::shared_ptr<Tile> currentTile = boardTiles_->at(c).at(r);
            if ((currentTile->getPieceOwner() != forWho) & (currentTile->getPieceOwner() != board_->getNullPlayer() ))
                // A player's playable tiles need to be adjacent to their opponent's tiles (not owned by the current player and not owned by null player).
                // We will check if these playable locations are flanked by one of this player's pieces as well later
                // and remove all moves which aren't
            {
                TilePoint thisTileLoc = currentTile->getPos();
                board_->getNeighbors(thisTileLoc, movableTiles);
                
                // remove all tiles with pieces already placed on them from the movable tile vector
                movableTiles.erase(std::remove_if(movableTiles.begin(), movableTiles.end(),
                                    [this](const std::shared_ptr<Tile>& tile) {
                                        return tile->getPieceOwner() != board_->getNullPlayer();
                                    }),
                                    movableTiles.end());
                
                // Further trim the list by removing tiles that are not flanked
       //         movableTiles.erase(std::remove_if(movableTiles.begin(), movableTiles.end(),
       //                             [this, &currentTile](const std::shared_ptr<Tile>& tile) {
       //                                 return !TileIsFlanked(currentTile);
       //                             }),
       //                             movableTiles.end());
            }
        }
    }
}

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

bool TurnLogic::TileIsFlanked(std::shared_ptr<Tile> tile, std::shared_ptr<Player> curPlayer) {
    /// Checks each direction around a tile for discs starting with the opponent's color and ending with the player's color
    
    // coordinates of 1 unit in each direction
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    
    TilePoint thisPos = tile->getPos();
    for (auto dir : nextDir) {
        // move 1 tile in the current direction
        TilePoint thisDir = TilePoint{thisPos.x + dir.x, thisPos.y + dir.y};
        bool foundOpp = false;
        
        while (board_->isValidPosition(thisDir)) {
            auto nextTile = board_->getBoardTile(thisDir);
        
            // if the current tile is owned by null player, it's a tile with no disc on it
            // sequences of the opponent's tiles ending with a blank tile aren't flanked
            if (nextTile->getPieceOwner() == board_->getNullPlayer()) {
                break;
            }

            // if the current tile is owned by the opponent
            if ((nextTile->getPieceOwner() != curPlayer) & (nextTile->getPieceOwner() != board_->getNullPlayer())) {
                foundOpp = true;
            }
            // if the current tile is owned by the current player, and the previous tile(s) were owned by the opponent
            else if ((foundOpp) & (nextTile->getPieceOwner() == curPlayer)) {
                // we've found a line of opponent tiles ending with a player tile = 'flank'
                return true;
            }
            // this should never happen since we've already checked for null tiles
            else {
                break;
            }

            // move 1 more tile in the current direction until one of the above conditions is met
            thisDir = TilePoint{thisDir.x + dir.x, thisDir.y + dir.y};
        }
    }
    return false;
}



void TurnLogic::getPlayableTiles(std::shared_ptr<Player> forWho, std::vector<std::shared_ptr<Tile>>& movableTiles) {
    // go over all the board tiles, finding all tiles owned by the opposing player
    for (int c = 0; c < boardTiles_->size(); c++) {
        for (int r = 0; r < boardTiles_->at(c).size(); r++) {
            std::shared_ptr<Tile> currentTile = boardTiles_->at(c).at(r);
            
            // check if the tile is owned by the opposing player (if it has an opponent's piece on it)
            if ((currentTile->getPieceOwner() != forWho) & (currentTile->getPieceOwner() != board_->getNullPlayer() ))
                // A player's playable tiles need to be adjacent to their opponent's tiles (not owned by the current player and not owned by null player).
            {
                TilePoint thisTileLoc = currentTile->getPos();
                
                // Start by retreiving this opponent tile's neighbors. These are the tiles the current player can theoretically place their tile on
                board_->getNeighbors(thisTileLoc, movableTiles);
            }
        }
    }
    
    // Remove all tiles with pieces already placed on them from the movable tile vector
    movableTiles.erase(std::remove_if(movableTiles.begin(), movableTiles.end(),
                        [this](const std::shared_ptr<Tile>& tile) {
                            return tile->getPieceOwner() != board_->getNullPlayer();
                        }),
                        movableTiles.end());
    
    
    // Remove all tiles that aren't flanked by the current player from the movable tiles
    movableTiles.erase(std::remove_if(movableTiles.begin(), movableTiles.end(),
                        [this, &forWho](const std::shared_ptr<Tile>& tile) {
                            return !TileIsFlanked(tile, forWho);
                        }),
                        movableTiles.end());
}


std::shared_ptr<Tile> TurnLogic::computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles) {
    // here is the tile the player clicked on
    TilePoint posClicked = board_->pixelToWorld(ix, iy);
    shared_ptr<Tile> tileClicked = board_->getBoardTile(posClicked);
    
    // find this tile in the playable tiles
    for (auto tile : movableTiles) {
        if (tile->posIsEqual(tileClicked)) {
            // they clicked on this one!
            return tile;
        }
    }
    return nullptr;
}


std::shared_ptr<Disc> TurnLogic::placePiece(std::shared_ptr<Player> forWho, std::shared_ptr<Tile> on) {
    TilePoint tileLoc = on->getPos();
    std::shared_ptr<Disc> thisDisc = std::make_shared<Disc>(tileLoc, forWho->getMyColor());
    board_->addPiece(forWho, thisDisc);
    return thisDisc;
}

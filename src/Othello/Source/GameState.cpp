//
//  GameState.cpp
//  Othello
//
//  Created by Michael Felix on 11/16/23.
//

#include "GameState.hpp"

using namespace std;
using namespace othello;


// othello will always have 2 players
const int GameState::NUM_GAME_PLAYERS = 2;

const float GameState::flip_interval_secs_ = 0.1;

GameState::GameState(shared_ptr<Player>& playerWhite, shared_ptr<Player>& playerBlack, shared_ptr<Board>& board)
    :   playerBlack_(playerBlack),
        playerWhite_(playerWhite),
        board_(board)
{
    
}

void GameState::getFlankingTiles(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles) {
    /// Checks each direction around a tile for discs starting with the opponent's color and ending with the player's color
    
    // coordinates of 1 unit in each direction
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    
    TilePoint thisPos = tile->getPos();
    for (int d = 0; d < nextDir.size(); d++) {
        // this will store all tiles flanked in each direction (if any)
        // this is the subvector, and there will be one subvector per direction
        flankedTiles.push_back(std::vector<shared_ptr<Tile>>());
        
        TilePoint dir = nextDir.at(d);
        // move 1 tile in the current direction
        TilePoint thisDir = TilePoint{thisPos.x + dir.x, thisPos.y + dir.y};
        bool foundOpp = false;
        
        while (board_->isValidPosition(thisDir)) {
            std::shared_ptr<Tile> nextTile = board_->getBoardTile(thisDir);
        
            // if the current tile is owned by null player, it's a tile with no disc on it
            // sequences of the opponent's tiles ending with a blank tile aren't flanked
            if (nextTile->getPieceOwner() == board_->getNullPlayer()) {
                break;
            }

            // if the current tile is owned by the opponent
            if ((nextTile->getPieceOwner() != curPlayer) && (nextTile->getPieceOwner() != board_->getNullPlayer())) {
                flankedTiles.at(d).push_back(nextTile);
                foundOpp = true;
                
                // this is an opponent's tile, but the next tile in this direction is null or invalid
                TilePoint finalPos = TilePoint{thisDir.x + dir.x, thisDir.y + dir.y};
                if (!board_->isValidPosition(finalPos) || board_->getBoardTile(finalPos)->getPieceOwner() == board_->getNullPlayer()) {
                    // clear flanked tiles as it's not a valid flank
                    flankedTiles.at(d).clear();
                    break;
                }
            }
            // if the current tile is owned by the current player, and the previous tile(s) were owned by the opponent
            else if ((foundOpp) && (nextTile->getPieceOwner() == curPlayer)) {
                // we've found a line of opponent tiles ending with a player tile = 'flank'
                // so keep the tiles we've added to the flanking tiles subvector
                break;
            }
            else {
                // clear the flanking tiles subvector
                flankedTiles.at(d).clear();
                break;
            }

            // move 1 more tile in the current direction until one of the above conditions is met
            thisDir = TilePoint{thisDir.x + dir.x, thisDir.y + dir.y};
        }
    }
}

unsigned int GameState::getPlayerTiles(shared_ptr<Player>& whose, std::vector<std::vector<std::shared_ptr<Tile>>>& playerTiles) {
    unsigned int numPlayerTiles = 0;
    RGBColor playerColor = whose->getMyColor();
    std::vector<std::vector<std::shared_ptr<Tile>>>* boardTiles_ = board_->getBoardTiles();
    for (unsigned int r = 0; r < boardTiles_->size(); r++) {
        playerTiles.push_back(std::vector<std::shared_ptr<Tile>>());
        for (unsigned int c = 0; c < boardTiles_->at(r).size(); c++) {
            std::shared_ptr<Tile> thisTile = boardTiles_->at(r)[c];
            if (thisTile->getPieceOwner()->getMyColor().isEqualTo(playerColor)) {
                playerTiles[r].push_back(thisTile);
                numPlayerTiles++;
            }
        }
    }
    return numPlayerTiles;
}

bool GameState::tileIsFlanked(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer) {
    std::vector<std::vector<std::shared_ptr<Tile>>> flankedTiles;
    getFlankingTiles(tile, curPlayer, flankedTiles);
    for (int o = 0; o < flankedTiles.size(); o++) {
        // each subvector represents one direction around this tile
        if (flankedTiles.at(o).size() > 0) {
            // tile is flanked in some direction
            return true;
        }
    }
    return false;
}

bool GameState::discIsStable(std::shared_ptr<Tile>& tile) {
    // to see if a disc is stable, we need to check tileIsFlanked on all the tiles around it
    RGBColor whiteColor = playerWhite_->getMyColor();
    RGBColor blackColor = playerBlack_->getMyColor();
    shared_ptr<Player> tileOwner = tile->getPieceOwner();
    
    std::shared_ptr<Player> opponent;
    if (tileOwner->getMyColor().isEqualTo(whiteColor)) { // does white control this tile?
         opponent = playerBlack_; // then opponent is black
    }
    else if (tileOwner->getMyColor().isEqualTo(blackColor)) { // does black control this tile?
        opponent = playerWhite_; // then opponent is white
    }
    else { // tile is blank (owned by null player)
        return false;
    }
    
    TilePoint tileLoc = tile->getPos();
    std::vector<std::shared_ptr<Tile>> neighborTiles;
    board_->getNeighbors(tileLoc, neighborTiles);
    
    for (std::shared_ptr<Tile> n : neighborTiles) {
        if (tileIsFlanked(n, opponent)) {
            return false;
        }
    }
    return true;
}

void GameState::getPlayableTiles(std::shared_ptr<Player>& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles) {
    // go over all the board tiles, finding all tiles owned by the opposing player
    std::vector<std::vector<std::shared_ptr<Tile>>>* boardTiles_ = board_->getBoardTiles();
    for (int r = 0; r < boardTiles_->size(); r++) {
        for (int c = 0; c < boardTiles_->at(r).size(); c++) {
            std::shared_ptr<Tile> currentTile = boardTiles_->at(r)[c];
            
            // check if the tile is owned by the opposing player (if it has an opponent's piece on it)
            if ((currentTile->getPieceOwner() != forWho) && (currentTile->getPieceOwner() != board_->getNullPlayer() ))
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
                        [this, &forWho](std::shared_ptr<Tile>& tile) {
                            return !tileIsFlanked(tile, forWho);
                        }),
                        movableTiles.end());
}


std::shared_ptr<Tile> GameState::computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles) {
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


std::shared_ptr<Disc> GameState::placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on) {
    RGBColor BLACK = RGBColor{0, 0, 0};
    RGBColor WHITE = RGBColor{1, 1, 1};
    
    TilePoint tileLoc = on->getPos();
    std::shared_ptr<Disc> thisDisc = std::make_shared<Disc>(tileLoc, forWho->getMyColor());
    board_->addPiece(forWho, thisDisc);
    
    std::vector<std::vector<std::shared_ptr<Tile>>> flankedTiles;
    // retreive which tiles are flanked by this new one
    getFlankingTiles(on, forWho, flankedTiles);
    
    // flip all flanked tiles
    for (auto dir: flankedTiles) {
        for (unsigned int i = 0; i < dir.size(); i++) {
            shared_ptr<Tile> tile = dir[i];
            if (forWho->getMyColor().isEqualTo(BLACK)) {
                tile->setOwner(playerBlack_);
                tile->getPiece()->setColorAfter(BLACK, flip_interval_secs_ * (i+1));
            } else if (forWho->getMyColor().isEqualTo(WHITE)) {
                tile->setOwner(playerWhite_);
                tile->getPiece()->setColorAfter(WHITE, flip_interval_secs_ * (i+1));
            }
        }
    }
    return thisDisc;
}


unsigned int GameState::placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on, bool returnInt) {
    RGBColor BLACK = RGBColor{0, 0, 0};
    RGBColor WHITE = RGBColor{1, 1, 1};
    
    TilePoint tileLoc = on->getPos();
    std::shared_ptr<Disc> thisDisc = std::make_shared<Disc>(tileLoc, forWho->getMyColor());
    board_->addPiece(forWho, thisDisc);
    
    std::vector<std::vector<std::shared_ptr<Tile>>> flankedTiles;
    // retreive which tiles are flanked by this new one
    getFlankingTiles(on, forWho, flankedTiles);
    
    // flip all flanked tiles
    unsigned int num_flipped = 0;
    for (auto dir: flankedTiles) {
        for (unsigned int i = 0; i < dir.size(); i++) {
            shared_ptr<Tile> tile = dir[i];
            if (forWho->getMyColor().isEqualTo(BLACK)) {
                tile->setOwner(playerBlack_);
                tile->getPiece()->setColorAfter(BLACK, flip_interval_secs_ * (i+1));
            } else if (forWho->getMyColor().isEqualTo(WHITE)) {
                tile->setOwner(playerWhite_);
                tile->getPiece()->setColorAfter(WHITE, flip_interval_secs_ * (i+1));
            }
            num_flipped++;
        }
    }
    return num_flipped;
}


void GameState::addGamePiece(TilePoint location, shared_ptr<Player>& whose, std::vector<std::shared_ptr<GraphicObject>>& allObjects) {
    shared_ptr<Disc> thisDisc = make_shared<Disc>(location, whose->getMyColor());
    board_->addPiece(whose, thisDisc);
    allObjects.push_back(thisDisc);
}


void GameState::addGamePiece(TilePoint location, shared_ptr<Player>& whose) {
    shared_ptr<Disc> thisDisc = make_shared<Disc>(location, whose->getMyColor());
    board_->addPiece(whose, thisDisc);
    // overloaded definition doesn't append to allObjects
}


bool GameState::isCornerTile(std::shared_ptr<Tile>& tile) {
    bool topRight = tile->getCol() == board_->getColsMax() && tile->getRow() == board_->getRowsMin();
    bool topLeft = tile->getCol() == board_->getColsMax() && tile->getRow() == board_->getRowsMax();
    bool bottomRight = tile->getCol() == board_->getColsMin() && tile->getRow() == board_->getRowsMin();
    bool bottomLeft = tile->getCol() == board_->getColsMin() && tile->getRow() == board_->getRowsMax();
    return topRight || topLeft || bottomLeft || bottomRight;
}

bool GameState::isCornerTile(TilePoint& tileLoc) {
    bool topRight = tileLoc.x == board_->getColsMax() && tileLoc.y == board_->getRowsMin();
    bool topLeft = tileLoc.x == board_->getColsMax() && tileLoc.y == board_->getRowsMax();
    bool bottomRight = tileLoc.x == board_->getColsMin() && tileLoc.y == board_->getRowsMin();
    bool bottomLeft = tileLoc.x == board_->getColsMin() && tileLoc.y == board_->getRowsMax();
    return topRight || topLeft || bottomLeft || bottomRight;
}


bool GameState::isCornerAdj(std::shared_ptr<Tile>& tile) {
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    TilePoint thisLoc;
    for (auto dir: nextDir) {
        thisLoc = TilePoint{tile->getRow() + dir.x, tile->getCol() + dir.y};
        if (isCornerTile(thisLoc))
            return true;
    }
    return false;
}


unsigned int GameState::numFrontierTiles(std::shared_ptr<Tile>& tile) {
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    TilePoint thisLoc;
    shared_ptr<Player> thisOwner;
    unsigned int numBlank = 0;
    for (auto dir: nextDir) {
        thisLoc = TilePoint{tile->getRow() + dir.x, tile->getCol() + dir.y};
        if (board_->isValidPosition(thisLoc)) {
            thisOwner = board_->getTileOwner(thisLoc);
            if (thisOwner == board_->getNullPlayer())
                numBlank++;
        }
    }
    return numBlank;
}

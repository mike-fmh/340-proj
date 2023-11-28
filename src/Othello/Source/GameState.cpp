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


GameState::GameState(shared_ptr<Player> playerWhite, shared_ptr<Player> playerBlack, shared_ptr<Board> board)
    :   startingPlayer_(playerWhite), // white always starts in othello
        currentPlayerTurn_(playerWhite),
        playerBlack_(playerBlack),
        playerWhite_(playerWhite),
        board_(board),
        boardTiles_(board->getBoardTiles())
{
    
}


GameState::GameState(const GameState& obj)
    :   startingPlayer_(obj.startingPlayer_), // white always starts in othello
        currentPlayerTurn_(obj.currentPlayerTurn_),
        playerBlack_(obj.playerBlack_),
        playerWhite_(obj.playerWhite_),
        board_(obj.board_),
        boardTiles_(vector<vector<shared_ptr<Tile>>>())
{
    // deep copy of board tiles, so editing the board in copied gamestate objects will not change the board in the original
    const vector<vector<shared_ptr<Tile>>>& sourceTiles = obj.board_->getBoardTiles();
    for (unsigned int r = 0; r < sourceTiles.size(); ++r) {
        boardTiles_.push_back(vector<shared_ptr<Tile>>());
        for (unsigned int c = 0; c < sourceTiles[r].size(); ++c) {
            // use tile's copy constructor to create a new shared ptr
            // the only attribute of Tile that needs a deep copy (new reference) would be owner_
            // but I don't plan on editing or addressing owner_ at all in copied gamestate objects, so we can ignore it
            boardTiles_[r].push_back(std::make_shared<Tile>(*sourceTiles[r][c]));
        }
    }
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

void GameState::getFlankingTiles(std::shared_ptr<Tile>& tile, Player& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles) {
    /// Checks each direction around a tile for discs starting with the opponent's color and ending with the player's color
    
    // coordinates of 1 unit in each direction
    std::vector<TilePoint> nextDir = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };
    
    RGBColor myColor = curPlayer.getMyColor();
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
            if ((!nextTile->getPieceOwner()->getMyColor().isEqualTo(myColor)) && (nextTile->getPieceOwner() != board_->getNullPlayer())) {
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
            else if ((foundOpp) && (!nextTile->getPieceOwner()->getMyColor().isEqualTo(myColor))) {
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

void GameState::getPlayerTiles(Player& whose, std::vector<std::vector<std::shared_ptr<Tile>>>& playerTiles) {
    bool tilesExistsInRow;
    RGBColor playerColor = whose.getMyColor();
    for (unsigned int r = 0; r < boardTiles_.size(); r++) {
        playerTiles.push_back(std::vector<std::shared_ptr<Tile>>());
        tilesExistsInRow = false;
        for (unsigned int c = 0; c < boardTiles_[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = boardTiles_[r][c];
            if (thisTile->getPieceOwner()->getMyColor().isEqualTo(playerColor)) {
                playerTiles[r].push_back(thisTile);
                tilesExistsInRow = true;
            }
        }
    }
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


bool GameState::tileIsFlanked(std::shared_ptr<Tile>& tile, Player& curPlayer) {
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


bool GameState::discIsPseudostable(std::shared_ptr<Tile>& tile, Player& curPlayer) {
    // to see if a disc is stable, we need to check tileIsFlanked on all the tiles around it
    RGBColor whiteColor = playerWhite_->getMyColor();
    std::shared_ptr<Player> opponent = playerWhite_; // default to opponent is white
    if (curPlayer.getMyColor().isEqualTo(whiteColor)) { // is it white's turn?
        opponent = playerBlack_; // then opponent is black
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
    for (int c = 0; c < boardTiles_.size(); c++) {
        for (int r = 0; r < boardTiles_[c].size(); r++) {
            std::shared_ptr<Tile> currentTile = boardTiles_[c][r];
            
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


// we need to overload this definition to use inside the AIplayer class when it passes itself to this function
void GameState::getPlayableTiles(Player& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles) {
    // go over all the board tiles, finding all tiles owned by the opposing player
    RGBColor myColor  = forWho.getMyColor();
    for (int c = 0; c < boardTiles_.size(); c++) {
        for (int r = 0; r < boardTiles_[c].size(); r++) {
            std::shared_ptr<Tile> currentTile = boardTiles_[c][r];
            
            // check if the tile is owned by the opposing player (if it has an opponent's piece on it)
            if ((!currentTile->getPieceOwner()->getMyColor().isEqualTo(myColor)) && (currentTile->getPieceOwner() != board_->getNullPlayer() ))
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
    TilePoint tileLoc = on->getPos();
    std::shared_ptr<Disc> thisDisc = std::make_shared<Disc>(tileLoc, forWho->getMyColor());
    board_->addPiece(forWho, thisDisc);
    
    std::vector<std::vector<std::shared_ptr<Tile>>> flankedTiles;
    // retreive which tiles are flanked by this new one
    getFlankingTiles(on, forWho, flankedTiles);
    
    // flip all flanked tiles
    for (auto dir: flankedTiles) {
        for (auto tile: dir) {
            if (forWho == playerBlack_) {
                tile->setOwner(playerBlack_);
                tile->getPiece()->setColor(0, 0, 0);
            } else {
                tile->setOwner(playerWhite_);
                tile->getPiece()->setColor(1, 1, 1);
            }
        }
    }
    return thisDisc;
}

std::shared_ptr<Disc> GameState::placePiece(Player& forWho, std::shared_ptr<Tile>& on) {
    TilePoint tileLoc = on->getPos();
    std::shared_ptr<Disc> thisDisc = std::make_shared<Disc>(tileLoc, forWho.getMyColor());
    board_->addPiece(forWho, thisDisc);
    
    std::vector<std::vector<std::shared_ptr<Tile>>> flankedTiles;
    // retreive which tiles are flanked by this new one
    getFlankingTiles(on, forWho, flankedTiles);
    
    // flip all flanked tiles
    RGBColor blackColor = playerBlack_->getMyColor();
    for (auto dir: flankedTiles) {
        for (auto tile: dir) {
            if (forWho.getMyColor().isEqualTo(blackColor)) {
                tile->setOwner(playerBlack_);
                tile->getPiece()->setColor(0, 0, 0);
            } else {
                tile->setOwner(playerWhite_);
                tile->getPiece()->setColor(1, 1, 1);
            }
        }
    }
    return thisDisc;
}

bool GameState::isCornerTile(std::shared_ptr<Tile>& tile) {
    bool topRight = tile->getCol() == board_->getColsMax() && tile->getRow() == board_->getRowsMin();
    bool topLeft = tile->getCol() == board_->getColsMax() && tile->getRow() == board_->getRowsMax();
    bool bottomRight = tile->getCol() == board_->getColsMin() && tile->getRow() == board_->getRowsMin();
    bool bottomLeft = tile->getCol() == board_->getColsMin() && tile->getRow() == board_->getRowsMax();
    return topRight || topLeft || bottomLeft || bottomRight;
}

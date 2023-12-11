#include "Board.hpp"

using namespace othello;


// traditional othello board is 8x8 tiles
const int Board::COLS_MIN_ = 1;
const int Board::COLS_MAX_ = 8;
const int Board::ROWS_MIN_ = 1;
const int Board::ROWS_MAX_ = 8;

// the actual "world" to render should larger than the game board
const int Board::PADDING_ = 1;

const int Board::X_MIN_ = 1 - PADDING_;
const int Board::X_MAX_ = 8 + PADDING_;
const int Board::Y_MIN_ = 1 - PADDING_;
const int Board::Y_MAX_ = 8 + PADDING_;

const float Board::WIDTH_ = (ROWS_MAX_ + PADDING_) - (ROWS_MIN_ - PADDING_);
const float Board::HEIGHT_ = (COLS_MAX_ + PADDING_) - (COLS_MIN_ - PADDING_);


Board::Board(RGBColor tileColor, std::shared_ptr<Player>& nullplayerRef)
    :   Object(0, 0, 0),
        GraphicObject(0, 0, 0),
        DEFAULT_TILE_COLOR_(tileColor),
        nullplayerRef_(nullplayerRef),
        allBoardTiles_(std::vector<std::vector<std::shared_ptr<Tile>>>())
{
    TilePoint thisPnt;
    for (int c = 1; c <= 8; c++) {
        allBoardTiles_.push_back(std::vector<std::shared_ptr<Tile>>());
        for (int r = 1; r <= 8; r++) {
            thisPnt = TilePoint{r, c};
            std::shared_ptr<Tile> thisTile = std::make_shared<Tile>(thisPnt, DEFAULT_TILE_COLOR_.red, DEFAULT_TILE_COLOR_.blue, DEFAULT_TILE_COLOR_.green, nullplayerRef);
            allBoardTiles_.at(c-1).push_back(thisTile);
        }
    }
}

std::vector<std::shared_ptr<Disc>> Board::getAllPieces() const {
    std::vector<std::shared_ptr<Disc>> pieces;
    for (unsigned int r = 0; r < allBoardTiles_.size(); r++) {
        for (std::shared_ptr<Tile> tile : allBoardTiles_[r]) {
            if (tile->getPiece() != nullptr) {
                pieces.push_back(tile->getPiece());
            }
        }
    }
    return pieces;
}

void Board::addPiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Disc>& piece) {
    // give the player ownership of the tile where we placed the new piece
    for (int c = 0; c < allBoardTiles_.size(); c++) {
        for (int r = 0; r < allBoardTiles_.at(c).size(); r++) {
            TilePoint tilePos = allBoardTiles_.at(c).at(r)->getPos();
            TilePoint piecePos = piece->getPos();
            if ((tilePos.x == piecePos.x) && (tilePos.y == piecePos.y)) { // is this board tile at the location we're placing the new piece?
                allBoardTiles_.at(c).at(r)->setOwner(forWho);
                allBoardTiles_.at(c).at(r)->setPiece(piece);
                return;
            }
        }
    }
}

void Board::setScalingRatios(int& paneWidth, int& paneHeight){
	float widthRatio = WIDTH_ / paneWidth;
	float heightRatio = HEIGHT_ / paneHeight;
	float maxRatio = fmax(widthRatio,heightRatio);

	pixelToWorldRatio = maxRatio;
	worldToPixelRatio = 1.f / pixelToWorldRatio;
	drawInPixelScale = pixelToWorldRatio;
	
	paneWidth = static_cast<int>(round(WIDTH_ * worldToPixelRatio));
	paneHeight = static_cast<int>(round(HEIGHT_ * worldToPixelRatio));
}

TilePoint Board::pixelToWorld(float ix, float iy)
{
	return TilePoint{	(int)round(Board::X_MIN_ + ix*Board::pixelToWorldRatio),
					(int)round(Board::Y_MAX_ - iy*Board::pixelToWorldRatio)
				};
}

TilePoint Board::pixelToWorld(const PixelPoint& pt)
{
	return TilePoint{	(int)round(Board::X_MIN_ + pt.x*Board::pixelToWorldRatio),
					(int)round(Board::Y_MAX_ - pt.y*Board::pixelToWorldRatio)
				};
}

PixelPoint Board::worldToPixel(float wx, float wy)
{
	return PixelPoint{(wx - Board::X_MIN_) * Board::worldToPixelRatio,
				 (Board::Y_MAX_ - wy) * Board::worldToPixelRatio};

}

PixelPoint Board::worldToPixel(const TilePoint& pt)
{
	return PixelPoint{(pt.x - Board::X_MIN_) * Board::worldToPixelRatio,
				 (Board::Y_MAX_ - pt.y) * Board::worldToPixelRatio};

}

std::shared_ptr<Tile> Board::getBoardTile(TilePoint& at) {
    float row = at.x;
    float col = at.y;
    if (row > Board::ROWS_MAX_)
        row = Board::ROWS_MAX_;
    if (col > Board::COLS_MAX_)
        col = Board::COLS_MAX_;
    
    // TilePoint coords go from 1-8, while boardTile->at() will range from 0-7
    // so in the boardTiles vector, all tile locations are -1 compared to them represented by TilePoints
    return allBoardTiles_.at(col - 1).at(row - 1);
}


std::shared_ptr<Player> Board::getTileOwner(TilePoint& at) {
    return getBoardTile(at)->getPieceOwner();
}


void Board::getNeighbors(TilePoint& tile, std::vector<std::shared_ptr<Tile>>& neighbors) {
    TilePoint tileLoc;
    if (tile.getCol() > 1) { // west
        tileLoc = TilePoint{tile.x - 1, tile.y};
        std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
        if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
            neighbors.push_back(curTile);
        if (tile.getRow() > 1) { // southwest
            tileLoc = TilePoint{tile.x - 1, tile.y - 1};
            std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
            if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
                neighbors.push_back(curTile);
        }
        if (tile.getRow() < Board::ROWS_MAX_) { // northwest
            tileLoc = TilePoint{tile.x - 1, tile.y + 1};
            std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
            if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
                neighbors.push_back(curTile);
        }
    }
    if (tile.getCol() < Board::Y_MAX_  - 1) { // east
        tileLoc = TilePoint{tile.x + 1, tile.y};
        std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
        if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
            neighbors.push_back(curTile);
        if (tile.getRow() > 1) { // southeast
            tileLoc = TilePoint{tile.x + 1, tile.y - 1};
            std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
            if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
                neighbors.push_back(curTile);
        }
        if (tile.getRow() < Board::ROWS_MAX_) { // northeast
            tileLoc = TilePoint{tile.x + 1, tile.y + 1};
            std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
            if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
                neighbors.push_back(curTile);
        }
    }
    if (tile.getRow() > 1) { // south
        tileLoc = TilePoint{tile.x, tile.y - 1};
        std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
        if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
            neighbors.push_back(curTile);
    }
    if (tile.getRow() < Board::X_MAX_ - 1) { // north
        tileLoc = TilePoint{tile.x, tile.y + 1};
        std::shared_ptr<Tile> curTile = getBoardTile(tileLoc);
        if (std::find(neighbors.begin(), neighbors.end(), curTile) == neighbors.end()) // if this location isn't already in the 'neighbors' vector
            neighbors.push_back(curTile);
    }
}

void Board::draw() const {
    // draw all constituent tiles
    for (int i = 0; i < allBoardTiles_.size(); i++) {
        for (auto tile : allBoardTiles_.at(i)) {
            if (tile != nullptr) {
                tile->draw();
            }
        }
    }
}

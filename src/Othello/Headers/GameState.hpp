//
//  GameState.hpp
//  Othello
//
//  keeps track of info about the current player's turn, like playable moves
//
//  Created by Michael Felix on 11/16/23.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <vector>
#include <memory>
#include "Board.hpp"
#include "Player.hpp"


namespace othello {
    class GameState {
    private:
        /// number of players in the game
        static const int NUM_GAME_PLAYERS;
        
        /// Animation time between flipping each sequential tile after a new piece is placed.
        static const float flip_interval_secs_;
        
        /// Reference to the game board
        std::shared_ptr<Board> board_;
        
        /// References to the white & black players used by board_
        std::shared_ptr<Player> playerBlack_;
        std::shared_ptr<Player> playerWhite_;
        
    public:
        /// Constructs a new GameState object, which stores references player objects & board object.
        /// Creating new GameState objects other than the main one (the one that's rendered in the game window) is done when an AI needs to hypothesize different possible moves that it can take.
        /// Each new GameState object needs a new separate board object, as the whole point of creating a new GameState object is that it contains a different board state than others,
        /// but also needs new separate player objects as well.
        /// @param playerWhite This GameState's reference to the White player.
        /// @param playerBlack This GameState's reference to the Black player.
        /// @param board The reference to this GameState's game board.
        GameState(std::shared_ptr<Player>& playerWhite, std::shared_ptr<Player>& playerBlack, std::shared_ptr<Board>& board);
        
        //disabled constructors & operators
        GameState(const GameState& obj) = delete;   // copy
        GameState() = delete;
        GameState(GameState&& obj) = delete;        // move
        GameState& operator = (GameState&& obj) = delete;        // move operator
        
        /// Populates the 'moveableTiles' vector with Tile references of possible moves for the given player.
        /// @param forWho The player whose possible moves to retrieve.
        /// @param movableTiles The vector to polulate with Tile references of their possible moves.
        void getPlayableTiles(std::shared_ptr<Player>& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Populates the flankedTiles vector a subvector for each direction, then appends tile sequences of opponent's tiles to each subvector in directions with valid flanks.
        void getFlankingTiles(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles);
        
        /// Given a tile, returns if the disc placed on it is flanked by another disc of the same color
        /// 'flanked' means that there's at least one disc between it and another similar disc of the opposing color in either direction (including diagonals)
        /// @param tile the tile to check is flanked by one of the current turn player's tiles
        /// @param curPlayer the player whose placing a piece (whose turn it is)
        bool tileIsFlanked(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer);
     
        /// Given a tile with a disc on it, returns whether that disc can be flipped (currently) by the opponent of 'curPlayer'.
        /// @param tile Reference to the Tile to check stability for.
        bool discIsStable(std::shared_ptr<Tile>& tile);
        
        /// GIven a screen x and y coord, translates it into a Board Tile.
        /// @param ix The screen x coord.
        /// @param iy The screen y coord.
        /// @param movableTiles Instead of searching on ALL board tiles, narrow down the tiles that are allowed to be clicked on to this vector of Tile references.
        std::shared_ptr<Tile> computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Place a new piece (disc) on the given tile. Per Othello rules, also flips all opposing tiles which are flanked by the given player.
        /// @param forWho Reference to the player who should own the new piece.
        /// @param on Reference to the tile to place the new piece on.
        std::shared_ptr<Disc> placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on);
        /// @param returnInt If a boolean is given to placePiece as the final param, the function will return how many opposing pieces this move flipped instead of a pointer to the new Disc it placed.
        unsigned int placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on, bool returnInt);
        
        /// Populates 'playerTiles' with all tiles owned by the player (their pieces/discs), and returns how many are owned in total.
        /// @param whose Reference to the player whose discs to count.
        /// @param playerTiles The empty vector to populate with Tile references.
        unsigned int getPlayerTiles(std::shared_ptr<Player>& whose, std::vector<std::vector<std::shared_ptr<Tile>>>& playerTiles);
        
        /// Given a tile, returns whether or not it's a corner tile.
        /// @param tile Reference to the tile to check is a corner tile.
        bool isCornerTile(std::shared_ptr<Tile>& tile);
        /// @param tileLoc Location of the tile to check is a corner tile.
        bool isCornerTile(TilePoint& tileLoc);
        
        /// Returns whether the given tile is adjacent to a corner tile.
        /// @param tile Tile to check adjacency to a corner tile.
        bool isCornerAdj(std::shared_ptr<Tile>& tile);
        
        /// Counts the number of blank tiles around the given tile.
        /// @param tile The tile to count the blank spaces around.
        unsigned int numFrontierTiles(std::shared_ptr<Tile>& tile);
        
        /// Add a piece to the board without flipping any pieces (used for initializing the game).
        /// @param location Where on the board to place ths new tile.
        /// @param whose The player who will control this piece.
        /// @param allObjects List of all game objects (to be rendered).
        void addGamePiece(TilePoint location, std::shared_ptr<Player>& whose, std::vector<std::shared_ptr<GraphicObject>>& allObjects);
        /// Overloaded definition that doesn't append to allObjects
        void addGamePiece(TilePoint location, std::shared_ptr<Player>& whose);
        
        /// Get a Tile on the board from its position
        /// @param at the TilePoint position of the tile to return
        inline std::shared_ptr<Tile> getBoardTile(TilePoint& at) const {
            return board_->getBoardTile(at);
        }
        
        /// Return a reference to this gamestate's game board (vector of all tiles).
        inline std::shared_ptr<Board> getBoard() {
            return board_;
        }
    };
}


#endif /* GameState_hpp */

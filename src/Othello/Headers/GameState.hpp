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
        
        static const float flip_interval_secs_;
        
        std::shared_ptr<Board> board_;
        
        std::shared_ptr<Player> playerBlack_;
        std::shared_ptr<Player> playerWhite_;
        
        /// player whose turn it is when the game starts
        std::shared_ptr<Player> startingPlayer_;
        
        /// player whose turn it currently is
        std::shared_ptr<Player> currentPlayerTurn_;
        
        /// pointer reference to the game board
        std::vector<std::vector<std::shared_ptr<Tile>>> boardTiles_;
        
    public:
        GameState(std::shared_ptr<Player> playerWhite, std::shared_ptr<Player> playerBlack, std::shared_ptr<Board> board);
        
        //disabled constructors & operators
        GameState(const GameState& obj) = delete;   // copy
        GameState() = delete;
        GameState(GameState&& obj) = delete;        // move
        GameState& operator = (GameState&& obj) = delete;        // move operator
        
        /// populates the vector referenced by moveableTiles with possible moves for the forWho player
        void getPlayableTiles(std::shared_ptr<Player>& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Populates the flankedTiles vector a subvector for each direction, then appends tile sequences of opponent's tiles to each subvector in directions with valid flanks.
        void getFlankingTiles(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles);
        
        /// Given a tile, returns if the disc placed on it is flanked by another disc of the same color
        /// 'flanked' means that there's at least one disc between it and another similar disc of the opposing color in either direction (including diagonals)
        /// @param tile the tile to check is flanked by one of the current turn player's tiles
        /// @param curPlayer the player whose placing a piece (whose turn it is)
        bool tileIsFlanked(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer);
     
        /// Given a tile with a disc on it, returns if that disc can be flipped (currently) by the opponent of curPlayer
        /// @param tile the tile in question
        /// @param curPlayer the player whose turn it currently is
        bool discIsStable(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer);
        
        /// GIven a screen x and y coord, translates it into a Board Tile
        /// @param ix the screen x coord
        /// @param iy the screen y coord
        /// @param movableTiles instead of searching on ALL board tiles, narrow down the tiles that are allowed to be clicked on to this vector
        std::shared_ptr<Tile> computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Place a new piece (disc) on the given tile
        /// @param forWho which player should own the new piece
        /// @param on the tile to place the new piece
        std::shared_ptr<Disc> placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on);
        /// @param returnInt if a boolean is given to placePiece as the final param, the function will return how many opposing pieces this move flipped instead of a pointer to the new Disc it placed
        unsigned int placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on, bool returnInt);
        
        unsigned int getPlayerTiles(std::shared_ptr<Player>& whose, std::vector<std::vector<std::shared_ptr<Tile>>>& playerTiles);
        
        /// Given a tile, returns whether or not it's on a board corner
        bool isCornerTile(std::shared_ptr<Tile>& tile);
        bool isCornerTile(TilePoint& tile);
        
        bool isCornerAdj(std::shared_ptr<Tile>& tile);
        
        bool gameIsOver();
        bool gameIsOver(std::vector<std::shared_ptr<Tile>>& playerMoves, bool isWhite);
        bool gameIsOver(std::vector<std::shared_ptr<Tile>>& blackMoves, std::vector<std::shared_ptr<Tile>>& whiteMoves);
        
        /// Add a piece to the board without flipping any pieces (used for initializing the game).
        /// @param location Where on the board to place ths new tile.
        /// @param whose The player who will control this piece.
        /// @param allObjects List of all game objects (to be rendered).
        void addGamePiece(TilePoint location, std::shared_ptr<Player>& whose, std::vector<std::shared_ptr<GraphicObject>>& allObjects);
        /// Overloaded definition doesn't append to allObjects
        void addGamePiece(TilePoint location, std::shared_ptr<Player>& whose);
        
        inline void passTurn(std::shared_ptr<Player> toWho) {
            currentPlayerTurn_ = toWho;
        }
        
        /// Get a Tile on the board from its position
        inline std::shared_ptr<Tile> getBoardTile(TilePoint& at) const {
            return board_->getBoardTile(at);
        }
        
        inline std::shared_ptr<Player> getCurrentPlayer() {
            return currentPlayerTurn_;
        }
        inline std::shared_ptr<Board> getBoard() {
            return board_;
        }
    };
}


#endif /* GameState_hpp */

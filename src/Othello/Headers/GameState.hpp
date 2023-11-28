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
        
        std::shared_ptr<Board> board_;
        
        std::shared_ptr<Player> playerBlack_;
        std::shared_ptr<Player> playerWhite_;
        
        /// player whose turn it is when the game starts
        std::shared_ptr<Player> startingPlayer_;
        
        /// player whose turn it currently is
        std::shared_ptr<Player> currentPlayerTurn_;
        
        /// pointer reference to the game board
        std::vector<std::vector<std::shared_ptr<Tile>>>* boardTiles_;
        
    public:
        GameState(std::shared_ptr<Player> playerWhite, std::shared_ptr<Player> playerBlack, std::shared_ptr<Board> board);
        
        //disabled constructors & operators
        GameState() = delete;
        GameState(const GameState& obj) = delete;    // copy
        GameState(GameState&& obj) = delete;        // move
        GameState& operator = (const GameState& obj) = delete;    // copy operator
        GameState& operator = (GameState&& obj) = delete;        // move operator
        
        /// populates the vector referenced by moveableTiles with possible moves for the forWho player
        void getPlayableTiles(std::shared_ptr<Player>& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles);
        void getPlayableTiles(Player& forWho, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Populates the flankedTiles vector a subvector for each direction, then appends tile sequences of opponent's tiles to each subvector in directions with valid flanks.
        void getFlankingTiles(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles);
        void getFlankingTiles(std::shared_ptr<Tile>& tile, Player& curPlayer, std::vector<std::vector<std::shared_ptr<Tile>>>& flankedTiles);
        
        /// Given a tile, returns if the disc placed on it is flanked by another disc of the same color
        /// 'flanked' means that there's at least one disc between it and another similar disc of the opposing color in either direction (including diagonals)
        /// @param tile the tile to check is flanked by one of the current turn player's tiles
        /// @param curPlayer the player whose placing a piece (whose turn it is)
        bool tileIsFlanked(std::shared_ptr<Tile>& tile, std::shared_ptr<Player>& curPlayer);
        bool tileIsFlanked(std::shared_ptr<Tile>& tile, Player& curPlayer);
     
        /// Given a tile with a disc on it, returns if that disc can be flipped (currently) by the opponent of curPlayer
        /// @param tile the tile in question
        /// @param curPlayer the player whose turn it currently is
        bool discIsPseudostable(std::shared_ptr<Tile>& tile, Player& curPlayer);
        
        /// GIven a screen x and y coord, translates it into a Board Tile
        /// @param ix the screen x coord
        /// @param iy the screen y coord
        /// @param movableTiles instead of searching on ALL board tiles, narrow down the tiles that are allowed to be clicked on to this vector
        std::shared_ptr<Tile> computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Place a new piece (disc) on the given tile
        /// @param forWho which player should own the new piece
        /// @param on the tile to place the new piece
        std::shared_ptr<Disc> placePiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Tile>& on);
        
        void getPlayerTiles(Player& whose, std::vector<std::vector<std::shared_ptr<Tile>>>& playerTiles);
        
        /// Given a tile, returns whether or not it's on a board corner
        bool isCornerTile(std::shared_ptr<Tile>& tile);
        
        inline void passTurn(std::shared_ptr<Player> toWho) {
            currentPlayerTurn_ = toWho;
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

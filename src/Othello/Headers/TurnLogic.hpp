//
//  TurnLogic.hpp
//  Othello
//
//  keeps track of info about the current player's turn, like playable moves
//
//  Created by Michael Felix on 11/16/23.
//

#ifndef TurnLogic_hpp
#define TurnLogic_hpp

#include <vector>
#include <memory>
#include "Board.hpp"
#include "Player.hpp"


namespace othello {
    class TurnLogic {
    private:
        /// number of players in the game
        static const int NUM_GAME_PLAYERS;
        
        std::shared_ptr<Board> board_;
        
        /// player whose turn it is when the game starts
        std::shared_ptr<Player> startingPlayer_;
        
        /// player whose turn it currently is
        std::shared_ptr<Player> currentPlayerTurn_;
        
        /// pointer reference to the game board
        std::vector<std::vector<std::shared_ptr<Tile>>>* boardTiles_;
        
    public:
        TurnLogic(std::shared_ptr<Player> playerWhite, std::shared_ptr<Player> playerBlack, std::shared_ptr<Board> board);
        
        //disabled constructors & operators
        TurnLogic() = delete;
        TurnLogic(const TurnLogic& obj) = delete;    // copy
        TurnLogic(TurnLogic&& obj) = delete;        // move
        TurnLogic& operator = (const TurnLogic& obj) = delete;    // copy operator
        TurnLogic& operator = (TurnLogic&& obj) = delete;        // move operator
        
        /// populates the vector referenced by moveableTiles with possible moves for the forWho player
        void getPlayableTiles(std::shared_ptr<Player> forWho, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Given a tile, returns if the disc placed on it is flanked by another disc of the same color
        /// 'flanked' means that there's at least one disc between it and another similar disc of the opposing color in either direction (including diagonals)
        /// @param tile the tile to check is flanked by one of the current turn player's tiles
        /// @param curPlayer the player whose placing a piece (whose turn it is)
        bool TileIsFlanked(std::shared_ptr<Tile> tile, std::shared_ptr<Player> curPlayer);
        
        /// GIven a screen x and y coord, translates it into a Board Tile
        /// @param ix the screen x coord
        /// @param iy the screen y coord
        /// @param movableTiles instead of searching on ALL board tiles, narrow down the tiles that are allowed to be clicked on to this vector
        std::shared_ptr<Tile> computeTileClicked(float ix, float iy, std::vector<std::shared_ptr<Tile>>& movableTiles);
        
        /// Place a new piece (disc) on the given tile
        /// @param forWho which player should own the new piece
        /// @param on the tile to place the new piece
        std::shared_ptr<Disc> placePiece(std::shared_ptr<Player> forWho, std::shared_ptr<Tile> on);
        
        inline std::shared_ptr<Player> getCurrentPlayer() {
            return currentPlayerTurn_;
        }
    };
}


#endif /* TurnLogic_hpp */

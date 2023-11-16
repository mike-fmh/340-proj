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
        void getPlayableTiles(std::shared_ptr<Player> forWho, std::vector<std::shared_ptr<Tile>>* movableTiles);
        
        inline std::shared_ptr<Player> getCurrentPlayer() {
            return currentPlayerTurn_;
        }
    };
}

#endif /* TurnLogic_hpp */

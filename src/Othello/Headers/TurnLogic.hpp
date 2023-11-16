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
#include "Tile.hpp"


namespace othello {
    class TurnLogic {
    private:
        /// number of players in the game
        const int NUM_GAME_PLAYERS;
        
        /// player whose turn it is when the game starts
        int startingPlayer_;
        
        /// player whose turn it currently is
        int currentPlayerTurn_;
        
        std::shared_ptr<std::vector<std::vector<std::shared_ptr<Tile>>>> boardTiles_;
        
    public:
        TurnLogic(int numPlayers, int startingPlayer, std::shared_ptr<std::vector<std::vector<std::shared_ptr<Tile>>>> board);
        
        //disabled constructors & operators
        TurnLogic() = delete;
        TurnLogic(const TurnLogic& obj) = delete;    // copy
        TurnLogic(TurnLogic&& obj) = delete;        // move
        TurnLogic& operator = (const TurnLogic& obj) = delete;    // copy operator
        TurnLogic& operator = (TurnLogic&& obj) = delete;        // move operator
        
    };
}

#endif /* TurnLogic_hpp */

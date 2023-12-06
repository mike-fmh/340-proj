//
//  AiMind.hpp
//  Othello
//
//  Created by Michael Felix on 12/5/23.
//

#ifndef AiMind_hpp
#define AiMind_hpp

#include "Board.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "GameState.hpp"

namespace othello {

    struct GamestateScore {
        
        /// Score based on mobility, which represents the amount of possible moves the player has.
        unsigned int mobilityScore;
        
        /// Score based on stability, which represents how many of the player's tiles aren't currently flanked (can't be flipped) by their opponent.
        unsigned int stabilityScore;
        
        /// Score based on how many corner pieces the player has.
        unsigned int cornerControlScore;
        
        /// Tiles adjacent to corners should result in lower scores, since they can allow the opponent to place on the corners
        int cornerAdjScore;
        
        /// The full gamestate score, computed by multiplying each score value by its corresponding weight and summing them together.
        int totalScore;
        
        int sum() {
            return cornerControlScore + stabilityScore + mobilityScore + cornerAdjScore;
        }
    };

    class AiMind {
    private:
        // weights for each factor based on their importance
        // used in computing Gamestate Advantage Score (measures how "good" a player's current gamestate is)
        const unsigned int MOBILITY_WEIGHT_;
        const unsigned int STABILITY_WEIGHT_;
        const unsigned int CORNER_WEIGHT_;
        const int CORNER_ADJ_WEIGHT_;
        
        RGBColor DEFAULT_TILE_COLOR_;
        
        static RGBColor WHITE, BLACK;
        
        int applyMinimaxMove_(bool maxing, unsigned int depth, std::shared_ptr<Tile>& thisMove, std::shared_ptr<Board>& oldBoard, int alpha, int beta);
        
    public:
        AiMind(unsigned int mobilityWeight, unsigned int stabilityWeight, unsigned int cornerWeight, int cornerAdjWeight, RGBColor defaultTileCol);
        
        
        unsigned int minimax(bool maximizing, unsigned int depth, std::shared_ptr<Player>& playerBlack, std::shared_ptr<Player>& playerWhite, std::shared_ptr<Board>& thisBoard, std::shared_ptr<GameState>& layout, int alpha, int beta);
        
        /// Run the AI's heuristic on all of its possible moves, and return the index of the best one in possibleMoves.
        /// @param forWho The player for whom to compute the best next move for.
        /// @param possibleMoves List of possible tiles that the player can choose as their next move.
        unsigned int bestMoveHeuristic(std::shared_ptr<Player>& forWho, std::shared_ptr<Board>& mainGameBoard, std::vector<std::shared_ptr<Tile>>& possibleMoves);
        unsigned int bestMoveHeuristic(std::shared_ptr<Player>& playerBlack, std::shared_ptr<Player>& playerWhite, std::shared_ptr<Board>& mainGameBoard, std::shared_ptr<GameState>& mainGameState, std::vector<std::shared_ptr<Tile>>& possibleMoves);

        unsigned int bestMoveMinimax(std::shared_ptr<Player>& playerBlack, std::shared_ptr<Player>& playerWhite, std::shared_ptr<Board>& mainGameBoard, std::shared_ptr<GameState>& mainGameState, std::vector<std::shared_ptr<Tile>>& possibleMoves, unsigned int depth);
        
        /// Called after a player places a piece on the board, this evaluates their gamestate advantage score.
        /// @param forWho The player for whom to calculate the gamestate advantage score (after they've placed a new piece).
        /// @param layout The gamestate from which to calculate the advantage score from.
        int evalGamestateScore(std::shared_ptr<Player>& forWho, std::shared_ptr<GameState>& layout);
        
        //disabled constructors & operators
        AiMind(AiMind&& obj) = delete;        // move
        AiMind(const AiMind& obj) = delete;
        AiMind& operator = (const AiMind& obj) = delete;    // copy operator
        AiMind& operator = (AiMind&& obj) = delete;        // move operator
        
    };

}

#endif /* AiMind_hpp */

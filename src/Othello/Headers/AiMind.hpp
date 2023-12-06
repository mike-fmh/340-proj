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
        
        unsigned int discScore;
        
        /// Score based on mobility, which represents the amount of possible moves the player has.
        unsigned int mobilityScore;
        
        /// Score based on stability, which represents how many of the player's tiles aren't currently flanked (can't be flipped) by their opponent.
        unsigned int stabilityScore;
        
        /// Score based on how many corner pieces the player has.
        unsigned int cornerControlScore;
        
        /// Tiles adjacent to corners should result in lower scores, since they can allow the opponent to place on the corners
        int cornerAdjScore;
        
        /// Score based on the number of blank tiles that surround this tile.
        /// We want to avoid placing pieces on tiles with too many blank tiles surrounding it, since it's more likely to be flipped by the opponent
        int frontierScore;
        
        /// The full gamestate score, computed by multiplying each score value by its corresponding weight and summing them together.
        int totalScore;
        
        int sum() {
            return discScore + cornerControlScore + stabilityScore + mobilityScore + cornerAdjScore + frontierScore;
        }
    };

    class AiMind {
    private:
        // weights for each factor based on their importance
        // used in computing Gamestate Advantage Score (measures how "good" a player's current gamestate is)
        const unsigned int MOBILITY_WEIGHT_;
        const unsigned int STABILITY_WEIGHT_;
        const unsigned int CORNER_WEIGHT_;
        const int NUM_FRONTIER_WEIGHT_;
        const int CORNER_ADJ_WEIGHT_;
        const unsigned int NUM_DISC_WEIGHT_;
        
        RGBColor DEFAULT_TILE_COLOR_;
        
        static RGBColor WHITE, BLACK;
        
        /// Helper function for minimax used to create a new gamestate object and place each hypothetical piece
        /// @param maxing Which mode minimax is currently in (maximizing or not).
        /// @param depth The current depth of the minimax tree
        /// @param thisMove The location of the new hypothetical move.
        /// @param oldBoard The board as it was before placing the new hypothetical move.
        /// @param alpha The current value for alpha (max) for minimax's alpha-beta pruning.
        /// @param beta The current value for beta (min) for minimax's alpha-beta pruning.
        int applyMinimaxMove_(bool maxing, unsigned int depth, std::shared_ptr<Tile>& thisMove, std::shared_ptr<Board>& oldBoard, int alpha, int beta);
        
    public:
        /// Creates a new AI object.
        /// Can compute best moves for either the black or white player.
        /// @param discWeight Weight for number of discs a player controls.
        /// @param mobilityWeight Weight for player mobility.
        /// @param stabilityWeight Weight for the number of stable tiles a player has.
        /// @param cornerWeight Weight for number of corner pieces a player has.
        /// @param cornerAdjWeight Weight for number of corner-adjacent tiles a player has.
        /// @param frontierWeight Weight for the number of blank tiles next to a player's tiles.
        /// @param defaultTileCol The default 'green' color of the game board.
        AiMind(unsigned int discWeight, unsigned int mobilityWeight, unsigned int stabilityWeight, unsigned int cornerWeight, int cornerAdjWeight, int frontierWeight, RGBColor defaultTileCol);
        
        
        int minimax(bool maximizing, unsigned int depth, std::shared_ptr<Player>& playerBlack, std::shared_ptr<Player>& playerWhite, std::shared_ptr<Board>& thisBoard, std::shared_ptr<GameState>& layout, int alpha, int beta);
        
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

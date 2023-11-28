//
//  AiPlayer.hpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#ifndef AiPlayer_hpp
#define AiPlayer_hpp

#include "GameState.hpp" // includes board, player, tile and more

namespace othello {

struct GamestateScore {
    
    /// Score based on mobility, which represents the amount of possible moves the player has.
    int mobilityScore;
    
    /// Score based on stability, which represents how many of the player's tiles aren't currently flanked (can't be flipped) by their opponent.
    int pseudostabilityScore;
    
    /// Score based on stability, which represents how many of the player's tiles couldn't possibly be flanked by their opponent for the remainder of the game.
    int stabilityScore;
    
    /// Score based on how many corner pieces the player has.
    int cornerControlScore;
    
    /// The full gamestate score, computed by multiplying each score value by its corresponding weight and summing them together.
    int totalScore;
    
    int sum() {
        return cornerControlScore + stabilityScore + pseudostabilityScore + mobilityScore;
    }
};

class AiPlayer : public Player {
private:
    static const int MOBILITY_WEIGHT_, STABILITY_WEIGHT_, CORNER_WEIGHT_;
    
    GamestateScore curPosScore_;
    
    /// Stores the game state for each hypothetical move (from all possible moves).
    /// While analyzing each possible move, this variable will be replaced with the gamestate for that move,
    /// and the move with the highest score will be used by the AI.
    
public:
    AiPlayer(RGBColor myColor);
    AiPlayer(RGBColor myColor, std::string name);
    
    /// Calculate the AI player's 'positional score' based on various factors regarding their current pieces and where they're placed.
    /// This gamestate positional score is not based on any fixed metric, but it supposed to give a sense of how strong their board presence is.
    GamestateScore evalGamestateScore(std::shared_ptr<GameState>& currentGamestate);
    
    /// Runs the AI's heuristic algorithm and returns a Tile point for the best possible move out of all valid ones.
    /// @param currentGamestate the game board state (where pieces are placed) before making any hypothetical moves.
    TilePoint findBestMove(std::shared_ptr<GameState>& currentGamestate);
    
    //disabled constructors & operators
    AiPlayer() = delete;
    AiPlayer(const AiPlayer& obj) = delete;    // copy
    AiPlayer(AiPlayer&& obj) = delete;        // move
    AiPlayer& operator = (const AiPlayer& obj) = delete;    // copy operator
    AiPlayer& operator = (AiPlayer&& obj) = delete;        // move operator
};

}

#endif /* AiPlayer_hpp */

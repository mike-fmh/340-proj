//
//  AiPlayer.hpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#ifndef AiPlayer_hpp
#define AiPlayer_hpp

#include "TurnLogic.hpp" // turnlogic includes board, player, tile and more


namespace othello {

/// The score the AI creates for each possible move based on the boardstate after the move
struct MoveScore {
    int mobility;
    int stability;
    int cornerControl;
    int totalScore;
};

class AiPlayer : public Player {
private:
    static const int MOBILITY_WEIGHT_, STABILITY_WEIGHT_, CORNER_WEIGHT_;
    
    /// Finds the number of possible moves based on the given boardstate
    void calculateMobility_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score);
    
    /// 
    void calculateStability_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score);
    
    /// Adds the amount of corners controlled by the AI to the score
    void calculateCornerControl_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score);
    
    /// Calculates the score multiplied by all the AI's weights for the given boardstate
    /// @param score the score to multiply by weights
    void computeTotalScore_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score);
    
public:
    AiPlayer(RGBColor myColor);
    AiPlayer(RGBColor myColor, std::string name);
    
    /// Calculate the score for the AI's position based on a given boardstate
    MoveScore evaluateBoardState(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard);
    
    //disabled constructors & operators
    AiPlayer() = delete;
    AiPlayer(const AiPlayer& obj) = delete;    // copy
    AiPlayer(AiPlayer&& obj) = delete;        // move
    AiPlayer& operator = (const AiPlayer& obj) = delete;    // copy operator
    AiPlayer& operator = (AiPlayer&& obj) = delete;        // move operator
};

}

#endif /* AiPlayer_hpp */

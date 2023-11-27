//
//  AiPlayer.cpp
//  Othello
//
//  Created by Michael Felix on 11/27/23.
//

#include "AiPlayer.hpp"

using namespace std;
using namespace othello;

// weights for each factor based on their importance
const int AiPlayer::MOBILITY_WEIGHT_ = 1;
const int AiPlayer::STABILITY_WEIGHT_ = 2;
const int AiPlayer::CORNER_WEIGHT_ = 3;

AiPlayer::AiPlayer(RGBColor color)
    :   Player(color)
{
    
}


void AiPlayer::calculateMobility_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score) {
    int legalMoves = 0; // TODO: count the number of legal moves available
    score.mobility = legalMoves;
}

void AiPlayer::calculateStability_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score) {
    
}

void AiPlayer::calculateCornerControl_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score) {
    
}

void AiPlayer::computeTotalScore_(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard, MoveScore& score) {
    score.totalScore = score.mobility * MOBILITY_WEIGHT_ +
                score.stability * STABILITY_WEIGHT_ +
                score.cornerControl * CORNER_WEIGHT_;
}


MoveScore AiPlayer::evaluateBoardState(std::vector<std::vector<std::shared_ptr<Tile>>>& hypotheticalBoard) {
    MoveScore score;
    calculateMobility_(hypotheticalBoard, score);
    calculateStability_(hypotheticalBoard, score);
    calculateCornerControl_(hypotheticalBoard, score);
    computeTotalScore_(hypotheticalBoard, score);
    return score;
}

AiPlayer::AiPlayer(RGBColor color, std::string name)
    :   Player(color, name)
{
    
}

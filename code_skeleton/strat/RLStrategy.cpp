#include "RLStrategy.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <fstream>


namespace sevens {

RLStrategy::RLStrategy() : 
    rng(std::chrono::system_clock::now().time_since_epoch().count()),
    epsilon(0.3), // Exploration rate
    alpha(0.1),   // Learning rate
    gamma(0.9)    // Discount factor
{
    // Initialize Q-table with zeros
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            Card card{suit, rank};
            q_values[card] = 0.0;
        }
    }
}

void RLStrategy::initialize(uint64_t playerID) {
    myID = playerID;
    current_state.clear();
    last_action = -1;
    last_reward = 0.0;
}

int RLStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout)
{
    if (hand.empty()) {
        return -1;
    }
    
    // Update state representation
    updateState(hand, tableLayout);
    
    // Find valid moves
    std::vector<int> validMoveIndices;
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
        if (isValidMove(hand[i], tableLayout)) {
            validMoveIndices.push_back(i);
        }
    }
    
    if (validMoveIndices.empty()) {
        last_action = -1;
        return -1;
    }
    
    // Epsilon-greedy action selection
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) < epsilon) {
        // Exploration: random move
        std::uniform_int_distribution<int> actionDist(0, validMoveIndices.size() - 1);
        last_action = validMoveIndices[actionDist(rng)];
    } else {
        // Exploitation: choose best Q-value
        double best_value = -std::numeric_limits<double>::infinity();
        int best_index = validMoveIndices[0];
        
        for (int idx : validMoveIndices) {
            const Card& card = hand[idx];
            // Get Q-value for this card
            double q_value = q_values[card];
            
            if (q_value > best_value) {
                best_value = q_value;
                best_index = idx;
            }
        }
        last_action = best_index;
    }
    
    // Remember the card we played
    if (last_action >= 0 && last_action < hand.size()) {
        last_card_played = hand[last_action];
    }
    
    return last_action;
}

void RLStrategy::observeMove(uint64_t playerID, const Card& playedCard) {
    // Learn from moves
    if (playerID == myID) {
        // Calculate immediate reward for our own move
        // Simple reward: +1 for playing a card
        double reward = 1.0;
        
        // Update Q-value
        q_values[last_card_played] += alpha * (reward - q_values[last_card_played]);
        last_reward = reward;
    } else {
        // Update our model of the game state based on other players' moves
        // For simplicity, just add the card to observed_cards
        observed_cards.insert(playedCard);
    }
}

void RLStrategy::observePass(uint64_t playerID) {
    if (playerID == myID) {
        // Negative reward for passing
        double reward = -0.5;
        
        // Update Q-value for passing
        // Since we don't have a card for passing, we don't update the Q-table
        last_reward = reward;
    }
}

std::string RLStrategy::getName() const {
    return "RLStrategy";
}



void RLStrategy::saveModel(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& pair : q_values) {
        file << pair.first.suit << " " << pair.first.rank << " " << pair.second << "\n";
    }
}

void RLStrategy::loadModel(const std::string& filename) {
    std::ifstream file(filename);
    int suit, rank;
    double value;
    while (file >> suit >> rank >> value) {
        Card card{suit, rank};
        q_values[card] = value;
    }
}







// Helper methods

bool RLStrategy::isValidMove(const Card& card, 
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) {
    
    // A card is valid if:
    // 1. It is a 7, or
    // 2. It is adjacent to a card already on table
    
    if (card.rank == 7) return true;
    
    auto suitIter = tableLayout.find(card.suit);
    if (suitIter == tableLayout.end()) return false;
    
    if (card.rank < 7) {
        // Check if card.rank+1 is on the table
        auto rankIter = suitIter->second.find(card.rank + 1);
        return (rankIter != suitIter->second.end() && rankIter->second);
    } else { // card.rank > 7
        // Check if card.rank-1 is on the table
        auto rankIter = suitIter->second.find(card.rank - 1);
        return (rankIter != suitIter->second.end() && rankIter->second);
    }
}

void RLStrategy::updateState(const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) {
    
    // Simple state representation as a string
    current_state.clear();
    
    // Add cards in hand to state
    for (const Card& card : hand) {
        current_state += std::to_string(card.suit) + "," + std::to_string(card.rank) + ";";
    }
    
    // Add table layout to state
    current_state += "|";
    for (const auto& suitPair : tableLayout) {
        for (const auto& rankPair : suitPair.second) {
            if (rankPair.second) {
                current_state += std::to_string(suitPair.first) + "," + 
                                 std::to_string(rankPair.first) + ";";
            }
        }
    }
}

} // namespace sevens
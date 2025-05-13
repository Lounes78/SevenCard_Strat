#include "GreedyStrategy.hpp"
#include <algorithm>
#include <iostream>

namespace sevens {

void GreedyStrategy::initialize(uint64_t playerID) {
    myID = playerID;
    // No special initialization for this minimal version
}




int GreedyStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout)
{
    if (hand.empty()) {
        return -1; // pass
    }
    
    // Find all valid moves first
    std::vector<std::pair<int, Card>> validMoves;
    
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
        const Card& card = hand[i];
        
        // Check if card is valid to play
        bool isValid = false;
        
        if (card.rank == 7) {
            isValid = true;
        } else if (card.rank < 7) {
            // Check if card.rank+1 is on the table
            auto suitIter = tableLayout.find(card.suit);
            if (suitIter != tableLayout.end()) {
                auto rankIter = suitIter->second.find(card.rank + 1);
                if (rankIter != suitIter->second.end() && rankIter->second) {
                    isValid = true;
                }
            }
        } else { // card.rank > 7
            // Check if card.rank-1 is on the table
            auto suitIter = tableLayout.find(card.suit);
            if (suitIter != tableLayout.end()) {
                auto rankIter = suitIter->second.find(card.rank - 1);
                if (rankIter != suitIter->second.end() && rankIter->second) {
                    isValid = true;
                }
            }
        }
        
        if (isValid) {
            validMoves.push_back({i, card});
        }
    }
    
    if (validMoves.empty()) {
        return -1; // No valid moves
    }
    
    // Greedy strategy - prioritize:
    // 1. Play cards farthest from 7 first (Aces and Kings)
    // 2. If tied, prefer higher suits (Spades > Hearts > Diamonds > Clubs)
    
    std::sort(validMoves.begin(), validMoves.end(), 
        [](const auto& a, const auto& b) {
            // Calculate distance from 7
            int distA = std::abs(a.second.rank - 7);
            int distB = std::abs(b.second.rank - 7);
            
            if (distA != distB) {
                return distA > distB; // Prefer cards farther from 7
            }
            
            // If same distance, prefer higher suit
            return a.second.suit > b.second.suit;
        });
    
    // Return index of the chosen card
    return validMoves[0].first;
}




void GreedyStrategy::observeMove(uint64_t /*playerID*/, const Card& /*playedCard*/) {
    // Ignored in minimal version
}

void GreedyStrategy::observePass(uint64_t /*playerID*/) {
    // Ignored in minimal version
}

std::string GreedyStrategy::getName() const {
    return "GreedyStrategy";
}

} // namespace sevens

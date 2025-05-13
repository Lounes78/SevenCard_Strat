#pragma once

#include "PlayerStrategy.hpp"
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <string>

namespace sevens {

// Custom hash function for Card
struct CardHash {
    size_t operator()(const Card& card) const {
        return (card.suit << 4) | card.rank;
    }
};

// Custom equality operator for Card
struct CardEqual {
    bool operator()(const Card& lhs, const Card& rhs) const {
        return lhs.suit == rhs.suit && lhs.rank == rhs.rank;
    }
};

/**
 * Reinforcement Learning strategy for Sevens
 */
class RLStrategy : public PlayerStrategy {
public:
    RLStrategy();
    ~RLStrategy() override = default;
    
    void initialize(uint64_t playerID) override;
    int selectCardToPlay(
        const std::vector<Card>& hand,
        const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) override;
    void observeMove(uint64_t playerID, const Card& playedCard) override;
    void observePass(uint64_t playerID) override;
    std::string getName() const override;
    
    void saveModel(const std::string& filename);
    void loadModel(const std::string& filename);


private:
    uint64_t myID;
    std::mt19937 rng;
    
    // RL parameters
    double epsilon; // Exploration rate
    double alpha;   // Learning rate
    double gamma;   // Discount factor
    
    // Q-table: map from card to value
    std::unordered_map<Card, double, CardHash, CardEqual> q_values;
    
    // State representation
    std::string current_state;
    
    // Memory of last action
    int last_action;
    Card last_card_played;
    double last_reward;
    
    // Set of observed cards played by others
    std::unordered_set<Card, CardHash, CardEqual> observed_cards;
    
    // Helper methods
    bool isValidMove(const Card& card, const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout);
    void updateState(const std::vector<Card>& hand, const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout);
};

} // namespace sevens
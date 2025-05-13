#pragma once

#include "Generic_game_mapper.hpp"
#include "../../strat/PlayerStrategy.hpp"
#include <random>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

namespace sevens {

/**
 * Enhanced Sevens simulation with strategy support:
 *  - Possibly internal mode or competition mode
 */
class MyGameMapper : public Generic_game_mapper {
public:
    MyGameMapper();
    ~MyGameMapper() = default;

    std::vector<std::pair<uint64_t, uint64_t>>
    compute_game_progress(uint64_t numPlayers) override;

    std::vector<std::pair<uint64_t, uint64_t>>
    compute_and_display_game(uint64_t numPlayers) override;
    
    std::vector<std::pair<std::string, uint64_t>>
    compute_game_progress(const std::vector<std::string>& playerNames) override;
    
    std::vector<std::pair<std::string, uint64_t>>
    compute_and_display_game(const std::vector<std::string>& playerNames) override;

    // Required by Generic_card_parser and Generic_game_parser
    void read_cards(const std::string& filename) override;
    void read_game(const std::string& filename) override;
    
    // Strategy management
    void registerStrategy(uint64_t playerID, std::shared_ptr<PlayerStrategy> strategy);
    bool hasRegisteredStrategies() const;


private:
    std::default_random_engine rng;
    std::unordered_map<uint64_t, std::shared_ptr<PlayerStrategy>> player_strategies;
    std::vector<std::vector<Card>> player_hands;
    // std::vector<std::vector<bool>> table_cards;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>> table_cards;


    
    // Helper methods
    void setupGame(uint64_t numPlayers);
    void dealCards();
    void initializeTable();
    bool isGameOver();
    void playRound(bool verbose);
    std::vector<Card> getValidMoves(size_t player_id);
    bool isValidMove(const Card& card);
    void makeMove(size_t player_id, const Card& card, bool verbose);
    std::vector<std::pair<uint64_t, uint64_t>> getFinalRankings();


};

} // namespace sevens

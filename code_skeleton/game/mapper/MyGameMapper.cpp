#include "MyGameMapper.hpp"
#include "../../card/MyCardParser.hpp"
#include "../parser/MyGameParser.hpp"

#include <iostream>
#include <stdexcept>
#include <chrono>  
#include <iostream>

#include <algorithm> 
#include <random>    

namespace sevens {

MyGameMapper::MyGameMapper() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = std::default_random_engine(static_cast<unsigned int>(seed)); // Store in member variable

    std::cout << "[MyGameMapper] Random engine seeded with: " << seed << std::endl;
}




void MyGameMapper::read_cards(const std::string& filename) {
    // Don't call the base class (it's pure virtual)
    MyCardParser cardParser;
    cardParser.read_cards(filename);
    cards_hashmap = cardParser.get_cards_hashmap();
}

void MyGameMapper::read_game(const std::string& filename) {
    // Don't call the base class (it's pure virtual)
    MyGameParser gameParser;
    gameParser.read_game(filename);
    table_cards = gameParser.get_table_layout();
}









// void MyGameMapper::read_cards(const std::string& filename) {
//     // Call parent class implementation, not sibling class
//     Generic_card_parser::read_cards(filename);
//     std::cout << "[MyGameMapper::read_cards] Loaded " << cards_hashmap.size() << " cards.\n";
// }

// void MyGameMapper::read_game(const std::string& filename) {
//     // Missing semicolon and incorrect call
//     Generic_game_parser::read_game(filename);
//     std::cout << "[MyGameMapper::read_game] Set up table layout.\n";
// }

bool MyGameMapper::hasRegisteredStrategies() const {
    return !player_strategies.empty(); // Check if we have any strategies
}

void MyGameMapper::registerStrategy(uint64_t playerID, std::shared_ptr<PlayerStrategy> strategy) {
    player_strategies[playerID] = strategy;
    std::cout << "[MyGameMapper::registerStrategy] Stored strategy for player " << playerID << ".\n";
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_game_progress(uint64_t numPlayers) {
    // Setup game state
    setupGame(numPlayers);
    
    // Play rounds until someone wins
    while (!isGameOver()) {
        playRound(false); // Play quietly
    }
    
    // Return results as (playerID, rank) pairs
    return getFinalRankings();
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_and_display_game(uint64_t numPlayers) {
    // Setup game state
    setupGame(numPlayers);
    
    // Play rounds until someone wins
    while (!isGameOver()) {
        playRound(true); // Play with output
    }
    
    // Return results as (playerID, rank) pairs
    return getFinalRankings();
}

std::vector<std::pair<std::string, uint64_t>> 
MyGameMapper::compute_game_progress(const std::vector<std::string>& playerNames) {
    // Convert to IDs and play the game
    auto results = compute_game_progress(playerNames.size());
    
    // Convert results back to names
    std::vector<std::pair<std::string, uint64_t>> namedResults;
    for (size_t i = 0; i < results.size(); i++) {
        namedResults.push_back({playerNames[results[i].first], results[i].second});
    }
    return namedResults;
}

std::vector<std::pair<std::string, uint64_t>>
MyGameMapper::compute_and_display_game(const std::vector<std::string>& playerNames) {
    // Convert to IDs and play the game
    auto results = compute_and_display_game(playerNames.size());
    
    // Convert results back to names
    std::vector<std::pair<std::string, uint64_t>> namedResults;
    for (size_t i = 0; i < results.size(); i++) {
        namedResults.push_back({playerNames[results[i].first], results[i].second});
    }
    return namedResults;
}

// Private helper methods
void MyGameMapper::setupGame(uint64_t numPlayers) {
    // Initialize player hands, table state, etc.
    // For each player, create empty hand
    player_hands.clear();
    player_hands.resize(numPlayers);
    
    // Deal cards to players
    dealCards();
    
    // Initialize table state
    initializeTable();
}

void MyGameMapper::dealCards() {
    // Create a deck of cards
    std::vector<Card> deck;
    for (const auto& pair : cards_hashmap) {
        deck.push_back(pair.second);
    }
    
    // Shuffle the deck
    std::shuffle(deck.begin(), deck.end(), rng);
    
    // Deal to players
    for (size_t i = 0; i < deck.size(); i++) {
        player_hands[i % player_hands.size()].push_back(deck[i]);
    }
}

void MyGameMapper::initializeTable() {
    // Clear the table layout
    table_cards.clear();
    
    // Start with no cards played
    for (int suit = 0; suit < 4; suit++) {
        // Initialize entries for this suit
        table_cards[suit] = std::unordered_map<uint64_t, bool>();
        
        // Set all ranks to false (not on table)
        for (int rank = 1; rank <= 13; rank++) {
            table_cards[suit][rank] = false;
        }
        
        // In Sevens, start with 7s on the table
        table_cards[suit][7] = true;
    }
}

bool MyGameMapper::isGameOver() {
    // Check if any player has no cards left
    for (const auto& hand : player_hands) {
        if (hand.empty()) {
            return true;
        }
    }
    return false;
}

void MyGameMapper::playRound(bool verbose) {
    // For each player
    for (size_t player_id = 0; player_id < player_hands.size(); player_id++) {
        // If player has no cards, skip
        if (player_hands[player_id].empty()) continue;
        
        // Get valid moves
        std::vector<Card> valid_moves = getValidMoves(player_id);
        
        // If no valid moves, skip
        if (valid_moves.empty()) {
            if (verbose) {
                std::cout << "Player " << player_id << " has no valid moves and passes.\n";
            }
            continue;
        }
        
        // Choose move based on strategy
        Card chosen_move;
        if (player_strategies.count(player_id) > 0) {
            // chosen_move = player_strategies[player_id]->chooseMove(valid_moves, table_cards);
        

            int move_index = player_strategies[player_id]->selectCardToPlay(valid_moves, table_cards);
            if (move_index >= 0 && move_index < valid_moves.size()) {
                chosen_move = valid_moves[move_index];
            }
        
        
        } else {
            // Default strategy: random
            std::uniform_int_distribution<size_t> dist(0, valid_moves.size() - 1);
            chosen_move = valid_moves[dist(rng)];
        }
        
        // Make the move
        makeMove(player_id, chosen_move, verbose);
    }
}

std::vector<Card> MyGameMapper::getValidMoves(size_t player_id) {
    std::vector<Card> valid_moves;
    
    // Check each card in the player's hand
    for (const Card& card : player_hands[player_id]) {
        // Check if card is valid (playable on table)
        if (isValidMove(card)) {
            valid_moves.push_back(card);
        }
    }
    
    return valid_moves;
}

bool MyGameMapper::isValidMove(const Card& card) {
    // In Sevens, a card is valid if:
    // 1. It is a 7, or
    // 2. It is adjacent to a card already on table
    
    if (card.rank == 7) return true;
    
    if (card.rank < 7) {
        // Check if rank+1 is on table
        return table_cards[card.suit][card.rank + 1];
    } else {
        // Check if rank-1 is on table
        return table_cards[card.suit][card.rank - 1];
    }
}

void MyGameMapper::makeMove(size_t player_id, const Card& card, bool verbose) {
    if (verbose) {
        std::cout << "Player " << player_id << " plays " << card << std::endl;
    }
    
    // Remove card from player's hand
    auto it = std::find_if(player_hands[player_id].begin(), player_hands[player_id].end(),
        [&card](const Card& c) { return c.suit == card.suit && c.rank == card.rank; });
    
    if (it != player_hands[player_id].end()) {
        player_hands[player_id].erase(it);
    }
    
    // Add card to table
    table_cards[card.suit][card.rank] = true;
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::getFinalRankings() {
    std::vector<std::pair<uint64_t, uint64_t>> rankings;
    
    // Count remaining cards for each player
    std::vector<std::pair<uint64_t, uint64_t>> player_cards;
    for (size_t i = 0; i < player_hands.size(); i++) {
        player_cards.push_back({i, player_hands[i].size()});
    }
    
    // Sort by card count (ascending)
    std::sort(player_cards.begin(), player_cards.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    // Assign ranks (1 = winner, etc.)
    for (size_t i = 0; i < player_cards.size(); i++) {
        rankings.push_back({player_cards[i].first, i + 1});
    }
    
    return rankings;
}

} // namespace sevens
#include "card/MyCardParser.hpp"
#include "game/mapper/MyGameMapper.hpp"
#include "game/parser/MyGameParser.hpp"
#include "strat/RandomStrategy.hpp"
#include "strat/GreedyStrategy.hpp"
#include "strat/RLStrategy.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace sevens;

int main() {
    // Create game mapper (which includes both card and game parser functionality)
    auto gameMapper = std::make_unique<MyGameMapper>();
    
    // Initialize cards and game state
    gameMapper->read_cards("");  // Empty string since we're generating cards, not reading from file
    gameMapper->read_game("");   // Empty string since we're using default game setup
    
    // Define number of players and their names
    const uint64_t numPlayers = 4;
    std::vector<std::string> playerNames = {
        "Alice", "Bob", "Charlie", "David"
    };

    gameMapper->registerStrategy(0, std::make_shared<GreedyStrategy>());
    
    // gameMapper->registerStrategy(1, std::make_shared<RLStrategy>());
    
    auto rlStrategy = std::make_shared<RLStrategy>();
    rlStrategy->loadModel("rl_model_60000.dat");  // Load the saved model
    gameMapper->registerStrategy(1, rlStrategy);  // Register with loaded weights
    
    gameMapper->registerStrategy(2, std::make_shared<RandomStrategy>());
    gameMapper->registerStrategy(3, std::make_shared<RandomStrategy>());
    
    std::cout << "=== Starting Sevens Game Simulation ===" << std::endl;
    
    // Run the game with display output 
    auto results = gameMapper->compute_and_display_game(playerNames);


    // Print final results
    std::cout << "\n=== Final Rankings ===" << std::endl;
    for (const auto& result : results) {
        std::cout << "Player " << result.first << " finished in position " 
                  << result.second << std::endl;
    }
    
    return 0;
}
#include "strat/RLStrategy.hpp"
#include "game/mapper/MyGameMapper.hpp"
#include "strat/RandomStrategy.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace sevens;

int main() {
    // Create our RL strategy
    auto rlStrategy = std::make_shared<RLStrategy>();
    
    // Number of training episodes
    const int EPISODES = 1000000;
    
    // Create strategies for other players
    auto randomStrategy = std::make_shared<RandomStrategy>();
    
    std::cout << "Starting training for " << EPISODES << " episodes..." << std::endl;
    
    // Track win rate over time
    int wins = 0;
    
    for (int episode = 0; episode < EPISODES; ++episode) {
        // Create game mapper
        auto gameMapper = std::make_unique<MyGameMapper>();
        
        // Initialize cards and game state
        gameMapper->read_cards("");
        gameMapper->read_game("");
        
        // Register strategies
        gameMapper->registerStrategy(0, rlStrategy);  // RL agent
        gameMapper->registerStrategy(1, randomStrategy);
        gameMapper->registerStrategy(2, randomStrategy);
        gameMapper->registerStrategy(3, randomStrategy);
        
        // Run simulation silently
        auto results = gameMapper->compute_game_progress(4);
        
        // Check if RL agent won
        for (const auto& result : results) {
            if (result.first == 0 && result.second == 1) {
                wins++;
                break;
            }
        }
        
        // Decrease exploration rate over time
        if (episode % 1000 == 0 && episode > 0) {
            // Save model periodically
            rlStrategy->saveModel("rl_model_" + std::to_string(episode) + ".dat");
            
            // Print progress
            double win_rate = static_cast<double>(wins) / episode;
            std::cout << "Episode " << episode << ", Win rate: " << win_rate << std::endl;
        }
    }
    
    // Save final model
    rlStrategy->saveModel("rl_model_final.dat");
    
    // Final stats
    double final_win_rate = static_cast<double>(wins) / EPISODES;
    std::cout << "Training complete. Final win rate: " << final_win_rate << std::endl;
    
    // Demonstrate the trained agent
    std::cout << "\nDemonstrating trained agent..." << std::endl;
    
    auto gameMapper = std::make_unique<MyGameMapper>();
    gameMapper->read_cards("");
    gameMapper->read_game("");
    
    std::vector<std::string> playerNames = {"RL Agent", "Random 1", "Random 2", "Random 3"};
    
    gameMapper->registerStrategy(0, rlStrategy);
    gameMapper->registerStrategy(1, randomStrategy);
    gameMapper->registerStrategy(2, randomStrategy);
    gameMapper->registerStrategy(3, randomStrategy);
    
    auto results = gameMapper->compute_and_display_game(playerNames);
    
    std::cout << "\nFinal Rankings:" << std::endl;
    for (const auto& result : results) {
        std::cout << result.first << ": Rank " << result.second << std::endl;
    }
    
    return 0;
}
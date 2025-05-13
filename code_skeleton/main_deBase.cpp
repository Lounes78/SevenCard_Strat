#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "card/MyCardParser.hpp"
#include "game/mapper/MyGameMapper.hpp"
#include "game/parser/MyGameParser.hpp"
#include "strat/RandomStrategy.hpp"
#include "strat/GreedyStrategy.hpp"
#include "strat/StrategyLoader.hpp"

using namespace sevens;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./sevens_game [mode] [optional libs...]\n";
        std::cout << "  Modes: internal, demo, competition\n";
        return 1;
    }
    
    std::string mode = argv[1];
    
    if (mode == "internal") {
        std::cout << "[main] Running internal mode with 4 random players\n";
        
        auto gameMapper = std::make_unique<MyGameMapper>();
        gameMapper->read_cards("");
        gameMapper->read_game("");
        
        // Internal mode uses 4 random players
        gameMapper->registerStrategy(0, std::make_shared<RandomStrategy>());
        gameMapper->registerStrategy(1, std::make_shared<RandomStrategy>());
        gameMapper->registerStrategy(2, std::make_shared<RandomStrategy>());
        gameMapper->registerStrategy(3, std::make_shared<RandomStrategy>());
        
        std::vector<std::string> playerNames = {
            "Random-0", "Random-1", "Random-2", "Random-3"
        };
        
        auto results = gameMapper->compute_and_display_game(playerNames);
        
        std::cout << "[main] Final Rankings:\n";
        for (const auto& result : results) {
            std::cout << "  " << result.first << " -> Rank " << result.second << "\n";
        }
    }
    else if (mode == "demo") {
        std::cout << "[main] Running demo mode with different strategy types\n";
        
        auto gameMapper = std::make_unique<MyGameMapper>();
        gameMapper->read_cards("");
        gameMapper->read_game("");
        
        // Demo uses built-in strategies
        gameMapper->registerStrategy(0, std::make_shared<RandomStrategy>());
        gameMapper->registerStrategy(1, std::make_shared<GreedyStrategy>());
        
        // Add additional built-in strategies if available
        
        std::vector<std::string> playerNames = {
            "Random", "Greedy"
        };
        
        auto results = gameMapper->compute_and_display_game(playerNames);
        
        std::cout << "[main] Final Rankings:\n";
        for (const auto& result : results) {
            std::cout << "  " << result.first << " -> Rank " << result.second << "\n";
        }
    }
    else if (mode == "competition") {
        if (argc < 3) {
            std::cerr << "Error: competition mode requires at least one strategy library.\n";
            std::cerr << "Usage: ./sevens_game competition [strategy1.so] [strategy2.so] ...\n";
            return 1;
        }
        
        std::cout << "[main] Running competition mode\n";
        
        auto gameMapper = std::make_unique<MyGameMapper>();
        gameMapper->read_cards("");
        gameMapper->read_game("");
        
        // Load strategies from shared libraries
        std::vector<std::string> playerNames;
        
        for (int i = 2; i < argc; i++) {
            std::string libPath = argv[i];
            std::cout << "Loading strategy from " << libPath << "...\n";
            
            try {
                auto strategy = StrategyLoader::loadFromLibrary(libPath);
                uint64_t playerID = i - 2;
                strategy->initialize(playerID);
                gameMapper->registerStrategy(playerID, strategy);
                std::string name = strategy->getName() + "-" + std::to_string(playerID);
                playerNames.push_back(name);
                std::cout << "Registered " << name << " successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Error loading strategy: " << e.what() << std::endl;
                return 1;
            }
        }
        
        std::cout << "Starting competition with " << playerNames.size() << " players...\n";
        
        auto results = gameMapper->compute_and_display_game(playerNames);
        
        std::cout << "[main] Competition Results:\n";
        for (const auto& result : results) {
            std::cout << "  " << result.first << " -> Final Rank " << result.second << "\n";
        }
    }
    else {
        std::cerr << "[main] Unknown mode: " << mode << std::endl;
        return 1;
    }
    
    return 0;
}
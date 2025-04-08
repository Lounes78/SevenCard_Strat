#include "MyGameMapper.hpp"
#include "MyCardParser.hpp"
#include <iostream>
#include <stdexcept>
#include <chrono>  

namespace sevens {

sevens::MyGameMapper::MyGameMapper() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    seed = std::default_random_engine(static_cast<unsigned int>(seed));

    std::cout << "[MyGameMapper] Random engine seeded with: " << seed << std::endl;
}

void MyGameMapper::read_cards(const std::string& filename) {
    MyCardParser::read_cards(filename);
    std::cout << "[MyGameMapper::read_cards] TODO: load or generate 52 cards.\n";
}

void MyGameMapper::read_game(const std::string& filename) {
    MyGameParser::read_game(filename)
    std::cout << "[MyGameMapper::read_game] TODO: set up table layout.\n";
}

bool MyGameMapper::hasRegisteredStrategies() const {
    return false;
}

void MyGameMapper::registerStrategy(uint64_t playerID, std::shared_ptr<PlayerStrategy> strategy) {
    // TODO: store the strategy so we can use it during simulation
    (void)playerID;
    (void)strategy;
    std::cout << "[MyGameMapper::registerStrategy] TODO: store player's strategy.\n";
}





std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_game_progress(uint64_t numPlayers)
{
    // TODO: implement a quiet simulation
    // Return e.g. (playerID, finalRank) for each player
    std::cout << "[MyGameMapper::compute_game_progress] TODO.\n";
    return {};
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_and_display_game(uint64_t numPlayers)
{
    // TODO: implement a verbose simulation
    std::cout << "[MyGameMapper::compute_and_display_game] TODO.\n";
    return {};
}





// std::vector<std::pair<std::string, uint64_t>>
// MyGameMapper::compute_game_progress(const std::vector<std::string>& playerNames)
// {
//     // Optional overload for name-based players
//     (void)playerNames;
//     std::cout << "[MyGameMapper::compute_game_progress(names)] TODO.\n";
//     return {};
// }

// std::vector<std::pair<std::string, uint64_t>>
// MyGameMapper::compute_and_display_game(const std::vector<std::string>& playerNames)
// {
//     // Optional overload for name-based players
//     (void)playerNames;
//     std::cout << "[MyGameMapper::compute_and_display_game(names)] TODO.\n";
//     return {};
// }





std::vector<std::pair<std::string, uint64_t>> 
sevens::MyGameMapper::compute_game_progress(const std::vector<std::string>& playerNames)
{
    std::cout << "[MyGameMapper::compute_game_progress(names)] Simulating game for " 
              << playerNames.size() << " players.\n";

    // Copie des noms de joueurs pour ne pas modifier l'entrée
    std::vector<std::string> names = playerNames;

    
    // Mélange aléatoire de la liste
    std::shuffle(names.begin(), names.end(), seed);

    
    std::vector<std::pair<std::string, uint64_t>> results;
    for (size_t i = 0; i < names.size(); ++i) {
        // Rang 1 pour le gagnant, 2 pour le deuxième, etc.
        results.emplace_back(names[i], i + 1);
    }
    return results;
}

std::vector<std::pair<std::string, uint64_t>>
sevens::MyGameMapper::compute_and_display_game(const std::vector<std::string>& playerNames)
{
    std::cout << "[MyGameMapper::compute_and_display_game(names)] Simulating game for " 
              << playerNames.size() << " players with detailed output.\n";

    // Copie des noms
    std::vector<std::string> names = playerNames;
    
    // Mélange aléatoire
    std::shuffle(names.begin(), names.end(), rng_);
    
    std::vector<std::pair<std::string, uint64_t>> results;
    // Simule la progression, affichant la position obtenue par chaque joueur
    for (size_t i = 0; i < names.size(); ++i) {
        uint64_t rank = i + 1;
        results.emplace_back(names[i], rank);
        std::cout << "Player " << names[i] << " takes rank " << rank << ".\n";
    }
    
    return results;
}

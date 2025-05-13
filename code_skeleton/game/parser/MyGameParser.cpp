#include "MyGameParser.hpp"
#include <iostream>

namespace sevens {

void MyGameParser::read_cards(const std::string& filename) {
    this->cards_hashmap.clear();

    for (int suit = 0; suit <= 3; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            // Card ID calculation (0-51)
            int cardId = suit * 13 + (rank - 1);
            
            Card card{suit, rank};
            this->cards_hashmap[cardId] = card;
            
            std::cout << "Added card ID " << cardId 
                      << ": Suit=" << suit 
                      << ", Rank=" << rank << std::endl;
        }
    }

    std::cout << "Created " << this->cards_hashmap.size()
              << " cards in total." << std::endl;
}

void MyGameParser::read_game(const std::string& filename) {
    this->table_layout.clear();

    // Initialize table for all suits
    for (int suit = 0; suit < 4; ++suit) {
        if (this->table_layout.find(suit) == this->table_layout.end()) {
            this->table_layout[suit] = std::unordered_map<uint64_t, bool>();
        }
        
        // Initialize all ranks as not on table
        for (int rank = 1; rank <= 13; ++rank) {
            this->table_layout[suit][rank] = false;
        }
        
        // In Sevens, game starts with only 7s on the table
        this->table_layout[suit][7] = true;
    }
    
    std::cout << "[MyGameParser::read_game] Table initialized with 7s of each suit.\n";
}

} // namespace sevens
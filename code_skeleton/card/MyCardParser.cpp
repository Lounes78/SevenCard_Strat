#include "MyCardParser.hpp"
#include <iostream>

namespace sevens {

void MyCardParser::read_cards(const std::string& filename) {
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

}
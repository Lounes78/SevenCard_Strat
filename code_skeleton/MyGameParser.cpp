#include "MyGameParser.hpp"
#include <iostream>

namespace sevens {


void MyGameParser::read_cards(const std::string& filename)  {

    this->cards_hashmap.clear() ; 

    for (int rank= 0 ; rank<=3 ; rank++  ){

        for(int suit=1 ;suit<=13 ; suit++){

            int cardId = suit * 13 + (rank - 1);

            Card card{suit , rank} ; 

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

    // initioalization 
    for (int suit = 0; suit < 4; ++suit) {
        if (this->table_layout.find(suit) == this->table_layout.end()) {
            this->table_layout[suit] = std::unordered_map<uint64_t, bool>();
        }
    }

        this->table_layout[2][7] = true ;


    
    std::cout << "[MyGameParser::read_game] Table initialized with 7s of each suit.\n";
    
    
}

} // namespace sevens
#include "MyGameParser.hpp"
#include "MyCardParser.hpp"
#include <iostream>

int main() {
    // Créer une instance de MyGameParser
    sevens::MyCardParser card;
    card.read_cards("");
    
    // Reading the game table
    sevens::MyGameParser parser;
    
    // Initialiser la table de jeu
    parser.read_game("");

    return 0 ;
}
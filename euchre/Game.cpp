//
//  Game.cpp
//  euchre
//
//  Created by Graham Eger on 12/18/18.
//  Copyright © 2018 Graham Eger. All rights reserved.
//

#include <stdio.h>
#include "Game.h"


namespace euchre {
    
    static std::string dir_to_str(Direction d) {
        switch (d) {
            case North:
                return "North";
            case South:
                return "South";
            case East:
                return "East";
            case West:
                return "West";
        }
    }
    
    static Direction left_of(Direction dir) {
        switch (dir) {
            case North:
                return East;
            case South:
                return West;
            case East:
                return South;
            case West:
                return North;
        }
    }
    
    static const Direction directions[4] = {North, East, South, West};
    
    Game::Game() {
        num_players = 0;
        players = {nullptr, nullptr, nullptr, nullptr};
        dealer = directions[rand() % 4];
        ns_score = 0;
        ew_score = 0;
    }
    
    void Game::play_game() {
        // have shuffled deck
        // deal cards
    }
    
    void Game::deal_to_players() {
        m.lock();
        Direction d = left_of(dealer);
        // go around twice
        for (size_t i = 0; i < 8; i++) {
            // even's deal two, oods deal 3
            players[d]->add_card(deck.pop());
            players[d]->add_card(deck.pop());
            if (i % 2 != 0) {
                players[d]->add_card(deck.pop());
            }
            d = left_of(dealer);
        }
        // done
        m.unlock();
    }
    
}

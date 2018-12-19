//
//  Game.hpp
//  euchre
//
//  Created by Graham Eger on 12/18/18.
//  Copyright © 2018 Graham Eger. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <string>
#include <atomic>
#include <mutex>
#include "Deck.h"
#include "Player.h"

namespace euchre {
    
    enum Direction {
        North = 0,
        East = 1,
        South = 2,
        West = 3
    };
    
    class Game {
    public:
        Game();
        void play_game();
        std::vector<Player*> get_players();
        bool add_player(Direction d, Player * p);
        bool remove_player(Direction d);
    private:
        void deal_to_players();
        
        std::mutex m;
        ShuffledDeck deck;
        std::atomic<size_t> num_players;
        std::array<Player*, 4> players;
        Direction dealer;
        size_t ns_score;
        size_t ew_score;
    };
}

#endif /* Game_h */

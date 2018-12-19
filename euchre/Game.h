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
        North = 0, // Captain
        East = 1,  // Captain
        South = 2,
        West = 3,
        Five = 5
    };
    
    class Round {
    public:
        Round(Direction dealer_in);
        Round();
        void set_trump(Suit trump_in, Direction caller_in);
        Direction play_trick(Direction leader);
        size_t ns_tricks;
        size_t ew_tricks;
        Suit trump;
        Direction caller;
        Direction dealer;
        std::array<Player*, 4> * players;
    };
    
    class Game {
    public:
        Game();
        void play_game();
        std::vector<Player*> get_players();
        bool add_player(Direction d, Player * p);
        bool remove_player(Direction d);
    private:
        Direction next_dealer();
        Direction current_dealer();
        void deal_to_players();
        std::pair<Direction, Suit> make_trump(Direction leader);
        void play_round();
        Direction play_trick(Direction leader);
        
        ShuffledDeck deck;
        Round current_round;
        std::atomic<size_t> num_players;
        std::array<Player*, 4> players;
        Direction starting_dealer;
        size_t round_number;
        size_t ns_score;
        size_t ew_score;
    };
}

#endif /* Game_h */

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
            case Five:
                return "Five";
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
            case Five:
                return Five;
        }
    }
    
    static const Direction directions[4] = {North, East, South, West};
    
    Game::Game() {
        num_players = 0;
        players = {nullptr, nullptr, nullptr, nullptr};
        starting_dealer = directions[rand() % 4];
        ns_score = 0;
        ew_score = 0;
    }
    
    Direction Game::current_dealer() {
        return directions[starting_dealer + round_number % players.size()];
    }
    
    void Game::play_game() {
        // have shuffled deck
        // deal cards
    }
    
    void Game::deal_to_players() {
        Direction d = left_of(current_dealer());
        // go around twice
        for (size_t i = 0; i < 8; i++) {
            // even's deal two, oods deal 3
            players[d]->add_card(deck.pop());
            players[d]->add_card(deck.pop());
            if (i % 2 != 0) {
                players[d]->add_card(deck.pop());
            }
            d = left_of(d);
        }
    }
    
    Round::Round(Direction dealer_in) {
        ns_tricks = 0;
        ew_tricks = 0;
        trump = None;
        dealer = dealer_in;
    }
    
    // go around until someone picks trump
    // assumes that players are following
    // SCREW THE DEALER
    std::pair<Direction, Suit> Game::make_trump(Direction leader) {
        Direction current_dir_up = leader;
        Card upcard = deck.upcard();
        // ask all 4 players if they'd like to tell the dealer
        // to 'pick it up'
        for (size_t i = 0; i < players.size(); i++) {
            bool rv = players[current_dir_up]->offer_trump(upcard,
                                                           current_dir_up == current_dealer());
            // if someone orders up trump in the first round
            // the dealer has to add and discard
            if (rv) {
                // let Player.cpp enforce player based rules: reneging, discarding cards, etc
                // overall game flow and rules will be enforced in Game.cpp
                
                // TODO discarded unused
                players[current_dir_up]->add_and_discard(upcard);
                return {current_dir_up, upcard.suit};
            }
            current_dir_up = left_of(current_dir_up);
        }
        // nobody wanted trump so we go again
        for (size_t i = 0; i < players.size(); i++) {
            Suit suit_chosen = players[current_dir_up]->offer_trump(current_dir_up == current_dealer());
            if (suit_chosen != None) {
                // this person ordered up trump!
                return {current_dir_up, suit_chosen};
            }
        }
        return {North, None};
    }
    
    // sets all the cards in everybody's hand as well as
    // the main game object's trump variable
    void Round::set_trump(Suit trump_in, Direction caller_in) {
        trump  = trump_in;
        caller = caller_in;
    }
    
    // actually play a trick
    Direction Round::play_trick(Direction leader) {
        std::array<Card, 4> table{};
        Direction up = leader;
        for (size_t i = 0; i < 4; i++) {
            table[up] = (*players)[up]->play_card();
        }
        // check the winner
        Card best_card = Card();
        Direction best_direction;
        for (size_t i = 0; i < 4; i++) {
            if (table[i] > best_card) {
                best_card = table[i];
                best_direction = directions[i];
            }
        }
        if (best_direction == North || best_direction == South) {
            ns_tricks++;
        } else {
            ew_tricks++;
        }
        return best_direction;
    }
    
    static bool same_team(Direction d1, Direction d2) {
        return (d1 + d2) != 2 || (d1 + d2) != 4;
    }
    
    // TODO make this async
    void Game::play_round() {
        // reset round
        current_round = Round();

        // make trump and store it as well as who called it
        auto ordered_up = make_trump(left_of(current_dealer()));
        current_round.set_trump(ordered_up.second, ordered_up.first);
        
        // start with the left of the dealer
        Direction leader = left_of(current_dealer());
        for (size_t i = 0; i < 5; i++) {
            leader = current_round.play_trick(leader);
        }
        // figure out winner and add points
        if (current_round.ns_tricks > current_round.ew_tricks) {
            // ns points
            if (current_round.ns_tricks == 5 ||
                current_round.caller == East ||
                current_round.caller == West) {
                ns_score += 2;
            } else {
                ns_score += 1;
            }
        } else {
            // ew points
            if (current_round.ew_tricks == 5 ||
                current_round.caller == North ||
                current_round.caller == South) {
                ew_score += 2;
            } else {
                ew_score += 1;
            }
        }
    }
}

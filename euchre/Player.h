//
//  Player.h
//  euchre
//
//  Created by Graham Eger on 12/18/18.
//  Copyright © 2018 Graham Eger. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <string>
#include <vector>
#include "Deck.h"

namespace euchre {
    
    const size_t MAX_HAND_SIZE = 5;
    
    class Hand {
        Hand() {
            d.reserve(MAX_HAND_SIZE);
        }
        // gotta check for dupes
        bool insert(const Card &t) {
            if (d.size() < MAX_HAND_SIZE && !contains(t)) {
                d.push_back(t);
                return true;
            }
            return false;
            std::sort(d.begin(), d.end());
        }
        bool contains(const Card &t) {
            for (auto card : d) {
                if (card.suit == t.suit && card.value == t.value) {
                    return true;
                }
            }
            return false;
        }
    private:
        std::vector<Card> d;
    };
    
    class Player {
    public:
        static const int MAX_HAND_SIZE = 5;
        
        // returns player's name
        virtual const std::string & get_name() const = 0;
        virtual void  add_card(const Card &c) = 0;
        virtual Card  play_card() = 0;
        virtual Suit  make_trump(const Card &upcard,
                                 bool is_dealer,
                                 int round,
                                 Suit order_up_suit);
        virtual bool offer_trump(const Card& upcard, bool is_dealer);
        virtual Suit offer_trump(bool is_dealer);
        // return the card that's removed
        virtual Card add_and_discard(const Card &updard) = 0;
    private:
        Hand hand;
    };
}

euchre::Player * Player_factory(const std::string &name, const std::string &type);

#endif /* Player_h */

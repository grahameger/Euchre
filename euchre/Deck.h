//
//  Deck.h
//  euchre
//
//  Created by Graham Eger on 12/18/18.
//  Copyright © 2018 Graham Eger. All rights reserved.
//

#ifndef Deck_h
#define Deck_h

#include <array>

namespace euchre {
    enum Suit {
        Hearts,
        Clubs,
        Diamonds,
        Spades,
        None
    };
    
    enum Value {
        Null = 0,
        Nine = 9,
        Ten = 10,
        Jack = 11,
        Queen = 12,
        King = 13,
        Ace = 14
    };
    
    enum Color {
        RED,
        BLACK,
        NONE
    };
    
    enum Category {
        OffsuitNotLed = 0,
        OffsuitLed = 1,
        Trump = 2
    };
    
    class Card {
    public:
        constexpr Card();
        Card(Suit suit_in, Value value_in);
        // methods
        void set_trump(Suit trump_in);
        void set_led(Suit led_in);
        bool is_trump() const;
        bool is_left() const;
        bool is_right() const;
        u_int16_t get_val() const;
        Color get_color() const;
        Color get_color(Suit suit_in) const;
        Suit left_suit() const;
        // comparison operators
        bool friend operator ==(const Card &a, const Card &b);
        bool friend operator !=(const Card &a, const Card &b);
        bool friend operator <(const Card &a, const Card &b);
        bool friend operator >(const Card &a, const Card &b);
        
        // properties
        Suit suit;
        Value value;
    protected:
        Category get_group() const;
        // only one trump is active for every card
        Suit trump;
        Suit led;
    };
    
    class Deck {
    public:
        constexpr Deck();
        static const size_t DECKSIZE = 24;
        Card deck[DECKSIZE];
    };
    
    class ShuffledDeck : public Deck {
    public:
        ShuffledDeck();
        Card pop();
    private:
        size_t remaining;
    };
}


#endif /* Deck_h */

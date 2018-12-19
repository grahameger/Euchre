
//
//  Deck.cpp
//  euchre
//
//  Created by Graham Eger on 12/18/18.
//  Copyright © 2018 Graham Eger. All rights reserved.
//

#include <stdio.h>
#include <algorithm>
#include <random>
#include <vector>
#include <array>
#include <iterator>
#include <string>
#include <atomic>
#include "Deck.h"


namespace std {
    template <> struct hash<euchre::Card>
    {
        size_t operator()(const euchre::Card & c) const
        {
            return std::hash<std::string>()(to_string(c.suit) + to_string(c.value));
        }
    };
}

namespace euchre {
    
    template<typename T, typename U>
    void swap(T& lhs, U& rhs) {
        T t = lhs;
        lhs = rhs;
        rhs = t;
    }
    
    template<typename RandomAccessIterator>
    void knuth_shuffle(RandomAccessIterator begin, RandomAccessIterator end) {
        static std::atomic<bool> lock = false;
        lock = true;
        for (size_t n = end - begin - 1; n >= 1; --n) {
            size_t k = std::rand() % (n + 1);
            if (k != n) {
                std::iter_swap(begin + k, begin + n);
            }
        }
        lock = false;
    }
    
    static constexpr std::array<Suit, 4> suits = {
        Hearts,
        Clubs,
        Diamonds,
        Spades
    };
    
    static constexpr std::array<Value, 6> values = {
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace
    };
    
    constexpr Card::Card() : suit(None), value(Null), trump(None), led(None) {}
    Card::Card(Suit suit_in, Value value_in) : suit(suit_in), value(value_in) {}
    
    void Card::set_trump(Suit trump_in) {
        trump = trump_in;
    }
    
    bool Card::is_trump() const {
        if (suit == trump) {
            return true;
        } else if (value == Jack && suit == left_suit()) {
            return true;
        }
        return false;
    }
    
    bool Card::is_left() const {
        return value == Jack && suit != trump && get_color() == get_color(trump);
    }
    
    bool Card::is_right() const {
        return value == Jack && suit == trump;
    }
    
    Color Card::get_color() const {
        return get_color(suit);
    }
    
    Color Card::get_color(Suit suit_in) const {
        switch (suit_in) {
            case Hearts:
                return RED;
            case Diamonds:
                return RED;
            case Spades:
                return BLACK;
            case Clubs:
                return BLACK;
            case None:
                return NONE;
        }
    }
    
    Suit Card::left_suit() const {
        switch (suit) {
            case Hearts:
                return Diamonds;
            case Diamonds:
                return Hearts;
            case Spades:
                return Clubs;
            case Clubs:
                return Spades;
            case None:
                return None;
        }
        return None;
    }
    
    Category Card::get_group() const {
        if (is_trump()) {
            return Trump;
        } else if (suit == led) {
            return OffsuitLed;
        } else {
            return OffsuitNotLed;
        }
    }
    
    bool operator==(const Card &a, const Card &b) {
        // a.led == b.led, a.suit == b.suit
        if ((a.suit == b.suit) && (a.suit != a.led != b.trump)) {
            return true;
        } else {
            return (a.suit == b.suit) && (a.value == b.value);
        }
    }
    
    bool operator!=(const Card &a, const Card &b) {
        return !(a == b);
    }
    
    bool operator<(const Card &a, const Card &b) {
        Category a_cat = a.get_group();
        Category b_cat = b.get_group();
        if (a_cat != b_cat) {
            return a_cat < b_cat;
        }
        if (a_cat == Trump) {
            if (b.is_right() || (b.is_left() && !a.is_right())) {
                return true;
            } else if (b.is_right() || (b.is_left() && !a.is_right())) {
                return true;
            }
        }
        return a.value < b.value;
    }
    
    bool operator>(const Card &a, const Card &b) {
        Category a_cat = a.get_group();
        Category b_cat = b.get_group();
        if (a_cat != b_cat) {
            return a_cat > b_cat;
        }
        if (a_cat == Trump) {
            if (b.is_right() || (b.is_left() && !a.is_right())) {
                return false;
            } else if (b.is_right() || (b.is_left() && !a.is_right())) {
                return false;
            }
        }
        return a.value > b.value;
    }
    
    // get a copy of the one that's on the top
    // swap it with the back and change the length
    Card ShuffledDeck::pop() {
        Card rv = deck[0];
        swap(deck[0], deck[remaining - 1]);
        remaining--;
        return rv;
    }
    
    ShuffledDeck::ShuffledDeck() : remaining(DECKSIZE) {
        // Knuth shuffle
        knuth_shuffle(std::begin(deck), std::end(deck));
    }
    
    constexpr Deck::Deck() {
        for (size_t i = 0; i < suits.size(); i++) {
            for (size_t j = 0; j < values.size(); j++) {
                deck[i * values.size() + j] = Card(suits[i], values[j]);
            }
        }
    }
}

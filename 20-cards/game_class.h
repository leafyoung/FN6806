#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

using std::array;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::generate;
using std::ostream;
using std::shuffle;
using std::vector;

#include "game_naive.h"

namespace GameClass {

struct Card {
  enum class Suit { Heart = 'H', Diamond = 'D', Club = 'C', Spade = 'S' };

  friend ostream &operator<<(ostream &os, const Suit &suit) {
    os << static_cast<char>(suit);
    return os;
  }

  friend int add() { return 3; }

  int c{1};
  Suit s{Suit::Heart};

  bool is_equal_point(const Card &d) { return c == d.c; }
  bool is_equal_suit(const Card &d) { return s == d.s; };
  friend ostream &operator<<(ostream &os, const Card &c) {
    os << c.s << c.c;
    return os;
  }
};

using Suit = Card::Suit;

class Game {
  vector<Card> cards;

public:
  Game(int n = 1) : cards(52 * n) {
    for (size_t n0 = 0; n0 < n; ++n0) {
      int vn = 0;
      for (auto s : {Suit::Heart, Suit::Diamond, Suit::Club, Suit::Spade}) {
        int c = 1;
        generate(next(cards.begin(), 52 * n0 + 13 * vn),
                 next(cards.begin(), 52 * n0 + 13 * (vn + 1)), [&c, &s]() {
                   return Card{c++, s};
                 });
        vn++;
      }
    }
    shuffle(cards.begin(), cards.end(), default_random_engine());
  }

  vector<Card> get_cards() const { return cards; }

  static bool is_serial(vector<Card> cards, int card_max = 13) {
    vector<GameNaive::Card> naive_cards(cards.size());
    transform(cards.begin(), cards.end(), naive_cards.begin(),
              [](const auto &card) { return GameNaive::Card{card.c}; });
    return GameNaive::is_serial(naive_cards, card_max);
  }
};
} // namespace GameClass

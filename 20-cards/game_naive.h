#pragma once

#include <array>
#include <iostream>
#include <vector>

namespace GameNaive {
using std::array;
using std::ostream;
using std::vector;

struct Card {
  size_t c;
};

ostream &operator<<(ostream &os, const Card &c);
inline size_t get_point(Card c);
inline size_t get_suit(Card c);

using Game = array<Card, 52>;
Game create_game();

bool is_equal_point(Card c, Card d);
bool is_equal_suit(Card c, Card d);
bool is_serial(vector<Card> cards, size_t card_max = 13);
} // namespace GameNaive

#pragma once

#include <array>
#include <iostream>
#include <vector>

namespace GameNaive {
using std::array;
using std::ostream;
using std::vector;

struct Card {
  int c;
};
ostream &operator<<(ostream &os, const Card &c);
inline int get_point(Card c);
inline int get_suit(Card c);

using Game = array<Card, 52>;
Game create_game();

bool is_equal_point(Card c, Card d);
bool is_equal_suit(Card c, Card d);
bool is_serial(vector<Card> cards, int card_max = 13);
} // namespace GameNaive

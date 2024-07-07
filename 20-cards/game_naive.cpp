#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

#include "game_naive.h"

namespace GameNaive {

using std::array;
using std::cout;
using std::generate;
using std::ostream;
using std::shuffle;
using std::vector;

std::ostream &operator<<(std::ostream &outs, const Card &c) {
  if (c.c >= 1 && c.c <= 13) {
    outs << 'H';
  } else if (c.c >= 14 && c.c <= 26) {
    outs << 'D';
  } else if (c.c >= 27 && c.c <= 39) {
    outs << 'C';
  } else if (c.c >= 40 && c.c <= 52) {
    outs << 'S';
  } else {
    throw std::logic_error("Unknown card serial");
  }
  outs << get_point(c); // Outputting the point value
  return outs;
}

inline int get_point(Card c) { return (c.c - 1) % 13 + 1; }
inline int get_suit(Card c) { return (c.c - 1) / 13; }

Game create_game() {
  using std::default_random_engine;
  Game game;
  int c = 1;
  generate(game.begin(), game.end(), [&c]() { return Card{c++}; });
  shuffle(game.begin(), game.end(), default_random_engine());
  return game;
}

bool is_equal_point(Card c, Card d) { return get_point(c) == get_point(d); }

bool is_equal_suit(Card c, Card d) { return get_suit(c) == get_suit(d); }

bool is_serial(vector<Card> cards, int card_max) {
  if (cards.size() < 2) {
    return false;
  }
  vector<int> points(cards.size());
  transform(cards.begin(), cards.end(), points.begin(),
            [](const auto &c) { return c.c; });
  sort(points.begin(), points.end());
  vector<int> diff(points.size());
  /*
  adjacent_difference() preserves the first element.
  We will only check from 2nd element onwards.
   4 6 9 13 18 19 19 15 10
   4 2 3  4  5  1  0 -4 -5
   */
  adjacent_difference(points.begin(), points.end(), diff.begin());

  // non-cyclic serial check: 3,4,5,6
  /*
  3   4	5	6
      3	4	5	6
      1	1	1
  */
  // Non-cyclic serial check
  const bool non_cyclic_serial = all_of(next(diff.begin()), diff.end(),
                                        [](const auto &v) { return v == 1; });
  // cyclic serial check:
  /*
  case 1:
  1	11	12	13
      1	11	12	13
      10	1	1
      =13-4+1

  case 2:
  1	13
      1	13
      12
      =13-2+1

  case 3:
  1	2	12	13
      1	2	12	13
      1	10	1
          =13-4+1
   */
  if (non_cyclic_serial)
    return non_cyclic_serial;

  const int gap = card_max - cards.size() + 1;
  const int gap_count = std::count(next(diff.begin()), diff.end(), gap);
  if (gap_count != 1)
    return false;

  const bool cyclic_serial =
      all_of(next(diff.begin()), diff.end(),
             [&](const auto &v) { return v == 1 || v == gap; });

  return cyclic_serial;
}
} // namespace GameNaive
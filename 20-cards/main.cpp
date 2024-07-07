#include <iostream>
using namespace std;

#include "game_class.h"
#include "game_naive.h"

int main() {
  cout << boolalpha;

  {
    using namespace GameNaive;
    auto cards = create_game();

    for (auto v : cards) {
      cout << v << ", ";
    }
    cout << endl;
    cout << Card({13}) << endl;
    cout << Card({27}) << endl;

    cout << is_equal_point({3}, {13 + 3}) << endl;
    cout << is_equal_suit({26 + 1}, {26 + 12}) << endl;

    cout << is_serial({{2}, {3}, {4}, {5}}) << endl;
    cout << is_serial({{13}, {14}, {15}, {16}}) << endl;
    cout << is_serial({{12}, {14}, {15}, {16}}) << endl;
  }
  {
    using namespace GameClass;
    Game g(1);

    for (auto v : g.get_cards()) {
      cout << v << ", ";
    }
    cout << endl;
    cout << Card{13, Suit::Heart} << endl;
    cout << Card{1, Suit::Club} << endl;

    auto c3h = Card{3, Suit::Heart};
    auto c4d = Card{4, Suit::Diamond};
    auto c5c = Card{5, Suit::Club};
    auto c4h = Card{4, Suit::Heart};

    cout << c4d.is_equal_point(c4h) << endl
         << c3h.is_equal_suit(c4d) << endl
         << g.is_serial({c3h, c4d, c5c}) << endl
         << g.is_serial({c5c, c4h}) << endl
         << g.is_serial({c3h, c5c}) << endl;
  }
}
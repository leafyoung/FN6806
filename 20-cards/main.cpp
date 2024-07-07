// https://replit.com/@YeKunlun/20-cards

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
    cout << "\n";
    cout << Card({13}) << "\n";
    cout << Card({27}) << "\n";

    cout << is_equal_point({3}, {13 + 3}) << "\n";
    cout << is_equal_suit({26 + 1}, {26 + 12}) << "\n";

    cout << is_serial({{2}, {3}, {4}, {5}}) << "\n";
    cout << is_serial({{13}, {14}, {15}, {16}}) << "\n";
    cout << is_serial({{12}, {14}, {15}, {16}}) << "\n";
  }
  {
    using namespace GameClass;
    Game g(1);

    for (auto v : g.get_cards()) {
      cout << v << ", ";
    }
    cout << "\n";
    cout << Card{13, Suit::Heart} << "\n";
    cout << Card{1, Suit::Club} << "\n";

    auto c3h = Card{3, Suit::Heart};
    auto c4d = Card{4, Suit::Diamond};
    auto c5c = Card{5, Suit::Club};
    auto c4h = Card{4, Suit::Heart};

    cout << c4d.is_equal_point(c4h) << "\n"
         << c3h.is_equal_suit(c4d) << "\n"
         << g.is_serial({c3h, c4d, c5c}) << "\n"
         << g.is_serial({c5c, c4h}) << "\n"
         << g.is_serial({c3h, c5c}) << "\n";
  }
}
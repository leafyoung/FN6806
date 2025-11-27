#pragma once

#include <iostream>
#include <vector>

namespace GameWithCard {

class Game {
public:
  class Card {
  public:
    char value;
    Card(char value) : value(value){};

    void game(const Game &g1) { std::cout << g1.cards.size() << '\n'; }
  };

private:
  std::vector<Card> cards;

public:
  Game() : cards({Card('H')}) {}
};
} // namespace GameWithCard
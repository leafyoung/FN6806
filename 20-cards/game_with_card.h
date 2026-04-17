#pragma once

#include <iostream>
#include <vector>

namespace GameWithCard {

class Game {
public:
  class Card {
    char suit;
    size_t value;

  public:
    Card(char suit, size_t value) : suit(suit), value(value){};
    friend std::ostream &operator<<(std::ostream &os, const Card &obj) {
      os << obj.suit << obj.value;
      return os;
    }
  };

private:
  std::vector<Card> cards;

public:
  Game(std::vector<Card> cards = {Card('H', 1)}) : cards(cards) {}
  const size_t count() { return this->cards.size(); }
};
} // namespace GameWithCard
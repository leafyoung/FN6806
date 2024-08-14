#include "04_test_input.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void test_user_input() {
  cout << __FUNCTION__ << ":\n";
  int size = -1;
  cout << "Input size of array: (type not a number or a negative number to "
          "cause exception)\n";
  try {
    cin >> size;
    if (!cin) // if input was not integer, cin becomes false
      throw 1;
    if (size < 0)
      throw logic_error("negative index!");
  } catch (const logic_error &e) {
    // cerr is the output channel for error messages
    cerr << "caught " << e.what() << "\n";
  }
  cout << "\n\n";
}

class ABC {
public:
  ~ABC() { cout << "~ABC()\n"; }
};

void test_dtor() {
  cout << __FUNCTION__ << ":\n";
  {
    ABC a;
    try {
      throw logic_error("");
    } catch (...) {
    }
    cout << "end of block\n";
  }
  {
    try {
      ABC a;
      throw logic_error("");
    } catch (...) {
    }
    cout << "end of block\n";
  }
  cout << "\n\n";
}
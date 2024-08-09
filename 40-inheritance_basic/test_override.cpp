#include <iostream>
using std::cout;

struct C {
  int c{0};
  int get_c() { return c; }
};

struct CPP : public C {
  int c{1};
  int get_c() { return c; }
  void print() {
    cout << c << "\n";          // derived class' member
    cout << C::c << "\n";       // base class' member
    cout << get_c() << "\n";    // derived class' member
    cout << C::get_c() << "\n"; // base class' member
  }
};

void test_override() {
  cout << "Test CPP: "
       << "\n";
  CPP cpp;
  cpp.print();
  cout << "\n";
}
#include <iostream>
#include <vector>
using namespace std;

struct ABC {
  int a{0};
};

void f() {
  auto *p = new vector<int>(300000);

  // without delete, there would be memory leak that accumulate with every
  // calling into the function till it burst the memory limit.
  // Reasons for not delete
  // 1. omission
  // 2. exception happened before the end of the function

  // delete p;
}

int main() {
  {
    int *p = new int(3);
    cout << *p << "\n";
    delete p;           // (1)
    cout << *p << "\n"; // (3) use after delete
    // delete p;           // (4) Error: double delete

    ABC *p2 = new ABC();
    cout << ++(p2->a) << "\n";
    delete p2; // (2)
  }            // if no delete at (1) and (2), there is memory leak.

  {
    // Use while loop simulate for
    // running f() many times
    // to manually cause the memory leak,
    // while (true)
    // f()
    ;
  }

  return 0;
}
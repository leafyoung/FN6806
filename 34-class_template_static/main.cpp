// https://replit.com/@YeKunlun/34-classtemplatestatic

#include <array>
#include <iostream>
#include <vector>
using namespace std;

template <typename T> class Widget {
  inline static int unsigned long counter = 0;

public:
  Widget() { counter++; }
  ~Widget() { counter--; }
  auto getCounter() { return counter; }
};

int main() {
  Widget<int> a;
  Widget<int> b;
  Widget<double> c;
  cout << a.getCounter() << '\n'; // 2
  cout << c.getCounter() << '\n'; // 1

  // Template speciailization, like Widget<int> or Widget<double> can be seen as
  // a standalone class.
}

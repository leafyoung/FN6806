// https://replit.com/@YeKunlun/3a-uniqueptr

#include <array>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

void f(int &t) { t += 3; }

// receive ownership
void f2(unique_ptr<int> p) { *p += 5; }

auto f3(unique_ptr<int> p) {
  *p += 7;
  // transfer ownership back
  return p;
}
class Widget {
  mutable unique_ptr<int> data;

public:
  Widget(int x) : data{make_unique<int>(x)} {}
  int get() { return *data; }
  // Widget(const Widget &other) : data(nullptr) { other.data.swap(data); }
  // Widget(const Widget &other) : data(make_unique<int>(*other.data)) {}
};

unique_ptr<int> return_int() { return make_unique<int>(3); }

int main() {
  {
    // 1. Initialization solo and in container.
    auto p_arr = make_unique<array<int, 3>>();
    (*p_arr)[0] = 0;
    (*p_arr)[1] = 1;
    (*p_arr)[2] = 2;
    auto p_vec = make_unique<vector<int>>(3);
    (*p_vec)[0] = 0;
    (*p_vec)[1] = 1;
    (*p_vec)[2] = 2;
  }

  {
    // 2. Pass unique_ptr in/out function.

    // not preferred, use make_unique
    unique_ptr<int> p1{new int(1)};

    // preferred
    unique_ptr<int> p2 = make_unique<int>(3);
    // or, use auto
    auto p3 = make_unique<int>(3);
    cout << "p1 (before): " << p1.get() << '\n';

    // assignment
    // p1 = p3; // error, not copyable
    p1 = move(p1); // ok, p1's object is moved to p1
    p3 = move(p1); // ok. p1's object is moved to p3, p1 becomes nullptr
                   // p3's resource is released
    cout << "p1 (after): " << p1.get() << '\n';

    // Pass the value to a function.
    f(*p2);
    cout << *p2 << '\n';
    cout << "p2: " << p2.get() << '\n';

    // f2(p2); // error, p2 is not copyable
    //  Use move to transfer ownership
    f2(move(p2)); // ok, p2 is moved to f2
    // p2 has become nullptr
    cout << "p2 (after): " << p2.get() << '\n';

    // move it in and receive it back
    p3 = f3(move(p3));
    cout << *p3 << '\n';
    p3.reset();
    cout << "p3 (after reset): " << p3.get() << '\n';
  }

  {
    // 3. unique_ptr in vector
    vector<unique_ptr<int>> vs;
    vs.emplace_back(make_unique<int>(3));
    vs.emplace_back(make_unique<int>(4));
    vs.emplace_back(make_unique<int>(5));

    // if you have a pointer outside, use `move`
    auto e4 = make_unique<int>(6);
    vs.emplace_back(move(e4));

    // int e5{6}; This will not work.
    // vs.emplace_back(move(e5));

    // auto v: vs cannot compile
    // because it will copy the value from elements in vs to v.
    // use reference (&) avoid copying
    for (auto const &v : vs) {
      cout << *v << ", ";
    }
    cout << '\n';
  }

  Widget w{315};
  cout << w.get() << '\n';

  {
    cout << "return unique_ptr from a function\n";
    auto x = return_int();
    cout << *x << '\n';
    cout << x.get() << '\n';
  }

  {
    int *p_x{nullptr};
    {
      int x{3};
      p_x = &x;
      cout << *p_x << '\n';
      cout << p_x << '\n';
    }
    {
      int y{4};
      cout << &y << '\n';
    }
    // p_x points to a memory location that does not belong to x anymore.
    // The same location could be overwritten by another variable
    // Note: compiler optimization may allocate y to another location.
    cout << *p_x << '\n';
  }
}
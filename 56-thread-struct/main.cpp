#include <iostream>
#include <thread>
#include <vector>
using namespace std;

struct ABC {
  vector<int> &x;
  size_t i;
  ABC(vector<int> &x, size_t i) : x{x}, i{i} {}
  void operator()() { cout << x[i] << endl; }
};

struct ABCD {
  int &x;
  ABCD(int &x) : x{x} {}
  void operator()() { cout << x << endl; }
};

int main() {
  // ABC abc(3);
  // abc();

  vector<int> vs{1, 2};

  {
    ABC def(vs, 0), ghi(vs, 1);
    thread t1(&ABC::operator(), &def);
    thread t2(&ABC::operator(), &ghi);

    t1.join();
    t2.join();
  }
  {
    ABCD def(vs[0]), ghi(vs[1]);
    thread t1(&ABCD::operator(), &def);
    thread t2(&ABCD::operator(), &ghi);

    t1.join();
    t2.join();
  }
}
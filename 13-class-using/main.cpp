#include <iostream>
using namespace std;

class PointX {
protected:
  int x;

public:
  PointX(int x) : x(x){};
  // ~PointX() = delete;
};

template <class T> T first(T a, T b) { return a; }

template <> int first(int a, int b) { return a; };

template <class T1, class T2> class Pair {
  T1 t1;
  T2 t2;

public:
  Pair(const T1 &t1, const T2 &t2) : t1(t1), t2(t2) {}
  bool operator==(const Pair &rhs) const;
};

template <class T1, class T2>
bool Pair<T1, T2>::operator==(const Pair<T1, T2> &rhs) const {
  return t1 == rhs.t1 && t2 == rhs.t2;
}

class PointX2 : public PointX {
public:
  // Use using to convert a protected member to a public member
  using PointX::x;
  // Use using to take base class's ctor
  using PointX::PointX;
};

template <class T> T abc(T a, T b) { return a; }

int main() {
  PointX px(3);
  PointX2 px2(4);
  cout << px2.x << endl;

  cout << first(1, 3) << endl;

  {
    Pair<int, int> pp(2, 3);
    auto p2 = pp;
    cout << (p2 == pp) << endl;
  }

  return 0;
}
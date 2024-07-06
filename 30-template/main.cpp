#include <array>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>
using namespace std;

struct Point {
  int x, y;

  // friend allows ostream to visit private members.
  friend std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  }

  bool operator<(const Point &other) const {
    // return x < other.x && y < other.y;
    // tie from <tuple> has the logic of comparing multiple data together.
    return tie(x, y) < tie(other.x, other.y);
  }
};

template <typename T> T mymax(T a, T b) { return a < b ? b : a; }

template <> Point mymax<Point>(Point a, Point b) {
  return a.x < b.x && a.y < b.y ? b : a;
}

template <size_t N, size_t DIM, typename T> class Boxes {
  // template <size_t N, size_t DIM = 3, typename T = double> class Boxes {
protected:
  std::array<T, N * DIM> data;

public:
  int get_size() { return data.size(); }
  // when we do not use get_dim(), this will get compiled.
  // As long as we have a use case, compiler to touch this part and find the
  // error.
  int get_dim() { return data.dim(); }
};

template <typename T> void print_container(const T &coll) {
  for (auto const &v : coll) {
    cout << v << ", ";
  }
  cout << endl;
}

int main() {
  // 1. Use function template
  cout << mymax(3, 2) << endl;
  cout << mymax(4.0, 3.0) << endl;
  cout << mymax("abc"s, "def"s) << endl;
  // cout << mymax(3, 2.0) << endl; // ERROR

  cout << (true > false) << endl;

  // C++11 allows us to skip the template argument
  // when it can deduce, but not for this one.
  // cout << mymax(3, 2.0) << endl;
  cout << mymax<double>(3, 2.0) << endl;

  // 2. Use class template
  // 4 3-dim boxes
  Boxes<4, 3, double> boxes_3d;
  cout << boxes_3d.get_size() << endl;

  // Below will have a compilation error.
  // cout << boxes_3d.get_dim() << endl;

  // 2 2-dim boxes
  Boxes<2, 2, double> boxes_2d;
  cout << boxes_2d.get_size() << endl;

  // add default type to template arguments to enable below
  // Boxes<4> boxes_3d_2;
  // cout << boxes_3d_2.get_size() << endl;

  // 3. Use specialized function template
  cout << mymax(Point{1, 2}, Point{3, 4}) << endl;

  print_container(vector<int>{3, 4});
  print_container(vector<string>{"def", "abc"});
  print_container(list<double>{3.0, 4.0});
}
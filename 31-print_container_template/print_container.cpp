#include <iostream>
#include <list>
#include <map>
#include <vector>
using namespace std;

#include "print_container.h"

void print_container_auto(auto const &coll) {
  for (auto const &v : coll) {
    cout << v << ", ";
  }
  cout << "\n";
}

void test_print_container() {
  {
    cout << "test print_container_basic()\n";

    print_container_basic(vector<int>{3, 4});
    print_container_basic(vector<string>{"def", "abc"});
    print_container_basic(list<double>{3.0, 4.0});
    cout << "\n";

    cout << "test print_container_auto()\n";
    print_container_auto(vector<int>{3, 4});
    print_container_auto(vector<string>{"def", "abc"});
    print_container_auto(list<double>{3.0, 4.0});
    cout << "\n";
  }
  {
    cout << "test print_container()\n";
    vector<double> vd{3.14, 8.1, 3.2, 1.0};
    print_container(vd);

    list<int> li{1, 2, 3, 5};
    print_container(li);

    print_container(vector<int>{3, 4});
    print_container(vector<string>{"def", "abc"});
    print_container(list<double>{3.0, 4.0});

    for (auto p : map<int, string>{{1, "abc"}, {2, "def"}}) {
      cout << p << " ";
      // print_container(p);
    }
    cout << "\n";

    auto m = map<int, string>{{1, "abc"}, {2, "def"}};
    print_container_var(m);
  }
}
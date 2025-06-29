#include <future>
#include <iostream>
using std::cout;
using std::launch;

void test_async() {
  cout << "test_async:\n";
  {
    auto x = async(launch::deferred, [] { return 7; });
    if (x.wait(), true) {
      cout << x.get();
    }
    auto y = async(launch::async, [] { return 9; });
    if (y.wait(), true) {
      cout << y.get();
    }
    cout << '\n';
  }
}
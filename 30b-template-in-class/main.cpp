#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class T, int n> void print(T array) {
  for (int i = 0; i < n; i++)
    cout << array[i] << endl;
}

// This allows class C to take a class template, i.e. vector, not sepecialized
// template vector<int>
template <template <class...> class C, class T, int n>
void print2(C<T> &array) {
  for (int i = 0; i < n; i++)
    cout << array[i] << endl;
}

int main() {
  vector<string> S{"Print"s, "function"s, "template"s};
  print<vector<string>, 3>(S);

  print2<vector, string, 3>(S);
  vector<double> S2{3, 2, 1};
  print2<vector, double, 3>(S2);

  return 0;
}

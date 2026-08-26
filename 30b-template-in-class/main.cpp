#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <utility>
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

template <typename TContainer>
TContainer fill(typename TContainer::value_type start,
                typename TContainer::value_type step,
                typename TContainer::value_type end) {
  size_t size = static_cast<size_t>((end - start) / step + 1);
  TContainer output(size);
  std::generate(std::begin(output), std::end(output), [&start, step]() {
    return std::exchange(start, start + step);
  });
  return output;
}

int main() {
  {
    // 1. Use function template
    vector<string> S{"Print"s, "function"s, "template"s};
    print<vector<string>, 3>(S);

    print2<vector, string, 3>(S);
    vector<double> S2{3, 2, 1};
    print2<vector, double, 3>(S2);
  }
  {
    // 2. Use function template to fill a container
    auto vec = fill<std::vector<int>>(0, 2, 10);
    auto list = fill<std::list<float>>(1, 0.3, 5);

    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    std::copy(list.begin(), list.end(),
              std::ostream_iterator<float>(std::cout, ", "));
    std::cout << std::endl;
  }

  return 0;
}

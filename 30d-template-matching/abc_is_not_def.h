#include <type_traits>

struct ABC {
  void abc() const {};
};

struct DEF {
  void def() const {};
};

template <typename T> void call_abc(const T &coll) {
  static_assert(std::is_same<T, ABC>::value, "T must be of ABC() class");
  return coll.abc();
}

template <typename T, typename U> bool compare(T &t, U &u) {
  static_assert(!std::is_same<T, U>::value, "T must be different from U");
}
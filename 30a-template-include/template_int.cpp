#include "template_int.h"

template <class T> A<T>::A(T a) : a(a) {}

template class A<int>;

template class A<double>;

template class A<bool>;

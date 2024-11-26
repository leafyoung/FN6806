// https://replit.com/@YeKunlun/3c-sharedptr

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// All class that to be used by shared_ptr and will return a share_ptr of itself
// shall inherit from enable_shared_from_this<T>

class Y : public enable_shared_from_this<Y> {
  int data = 0;

public:
  shared_ptr<Y> getY() { return shared_from_this(); }
  shared_ptr<Y> getY2() {
    // We shall not use below
    // you will see the use_count NOT grow with it
    return shared_ptr<Y>(this);
  }

  ~Y() { cout << "~Y()\n"; }
  void incr() { ++data; }
  int get() { return data; }
};

void f(Y &y) { y.incr(); }

void f(shared_ptr<Y> y) {
  y->incr();
  cout << "in f: " << y.use_count() << "\n";
}

int main() {
  {
    // 1. Initialization
    // not preferred
    shared_ptr<Y> pY(new Y);
    cout << "pY is released after this.\n";

    // preferred with make_shared
    auto p = make_shared<Y>();
    shared_ptr<Y> q{p}; // copy from p
    cout << q.use_count() << "\n";
    cout << p.use_count() << "\n";
  } // pY is released here

  {
    // 2. Check use count
    auto p = make_shared<Y>();
    shared_ptr<Y> q{p}; // copy from p

    cout << "We shall have 2 use_count()\n";
    cout << p.use_count() << ", " << q.use_count() << "\n";
    cout << p.get() << ", " << q.get() << "\n";

    // p.reset(); // cause runtime error in later use of p.

    // we shall see p.use_count() increase while in f.
    f(p);
    cout << "after f: " << p.use_count() << "\n";

    f(*p);
    cout << "p->get(): " << p->get() << "\n";
    f(*q);
    cout << "p and q->get(): " << p->get() << ", " << q->get() << "\n";
    q.reset();
    cout << "after q.reset(): " << p.use_count() << "\n";
  }

  {
    auto p = make_shared<Y>();
    // return a shared from a shared_ptr
    shared_ptr<Y> x = p;
    shared_ptr<Y> z = p->shared_from_this();
    cout << "We shall have 3 use_count()\n";
    assert(p == z);
    assert(p == x);

    cout << p.use_count() << " == " << x.use_count() << " == " << z.use_count()
         << "\n";
    cout << p.get() << " == " << z.get() << " == " << x.get() << "\n";
  }

  // below code may cause runtime error
  {
    auto p = make_shared<Y>();
    // return a shared from a share_ptr
    shared_ptr<Y> x = p->getY();
    shared_ptr<Y> z = p->getY();
    cout << x.use_count() << " == " << z.use_count() << "\n";
    cout << p.get() << " == " << z.get() << " == " << x.get() << "\n";
    cout.flush();
  } // double free corruption error if we replace the above with ->getY2()
    // each shared_ptr think it owns the object and frees it although it has
    // been freed by others already.

  return 0;
}
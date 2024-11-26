#include <iostream>

using std::cout;

class D {
protected:
  D() { cout << "D()\n"; }
  D(int x) { cout << "D(x)\n"; }
};
class E : public D {
public:
  // ctor case 1 :
  // It needs the default ctor D() in base class D
  E() { cout << "E()\n"; }
  // ctor case 2 :
  // E() : D() { cout << "E()\n"; }
  // ctor case 3 :
  E(int x) : D(x) { cout << "E()\n"; }
};
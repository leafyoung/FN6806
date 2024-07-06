#include <iostream>
using namespace std;

#include "comparable.h"
#include "pricing.h"

int main() {
  { test_comparable(); }
  {
    cout << "Test Pricing (hasDelta)" << endl;
    using pricing::Futures;
    using pricing::Swaps;

    // We use discount factor 1 for cash-settled futures/swaps.
    // Because the PnL is daily instance, discount factor is 1.0.
    Futures f(1000, 100, 95, 1);
    cout << "Futures: underlying: " << f.get_underlying() << endl;
    cout << "Futures: pv: " << f.pv(f.get_underlying()) << endl;
    cout << "Futures: delta: " << f.delta() << endl;

    Swaps s(1000, 100, 95, 80, 0.3, 1);
    cout << "Swap: underlying: " << s.get_underlying() << endl;
    cout << "Swap: pv: " << s.pv(s.get_underlying()) << endl;
    cout << "Swap: delta: " << s.delta() << endl;
  }
}
// https://replit.com/@YeKunlun/49-crtpextenddelta

#include <iostream>
using namespace std;

#include "comparable.h"
#include "pricing.h"

int main() {
  {
    test_comparable();
  }
  {
    cout << "Test Pricing (hasDelta)\n";
    using pricing::Futures;
    using pricing::Swaps;

    // We use discount factor 1 for cash-settled futures/swaps.
    // Because the PnL is daily instance, discount factor is 1.0.
    Futures f(1000, 100, 95, 1);
    cout << "Futures: underlying: " << f.get_underlying() << '\n';
    cout << "Futures: pv: " << f.pv(f.get_underlying()) << '\n';
    cout << "Futures: delta: " << f.delta(0.0001) << '\n';

    Swaps s(1000, 100, 95, 80, 0.3, 1);
    cout << "Swap: underlying: " << s.get_underlying() << '\n';
    cout << "Swap: pv: " << s.pv(s.get_underlying()) << '\n';
    cout << "Swap: delta: " << s.delta(0.0001) << '\n';
  }
}
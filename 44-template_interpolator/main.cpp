// https://replit.com/@YeKunlun/44-templateinterpolator

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "inter1.h"
#include "inter1p.h"
#include "inter2.h"

int main() {
  {
    using namespace T1;
    TermStructure<LinearInterpolator, double, double> linear({1, 2, 3},
                                                             {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructure<SquaredInterpolator, double, double> squared({1, 2, 3},
                                                               {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
  }
  {
    using namespace T2;

    TermStructure<T1::LinearInterpolator<double, double>> linear({1, 2, 3},
                                                                 {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructure<T1::SquaredInterpolator<double, double>> squared({1, 2, 3},
                                                                   {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
  }
  {
    using namespace T1p;
    TermStructure<LinearInterpolator, double, double> linear({1, 2, 3},
                                                             {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructure<SquaredInterpolator, double, double> squared({1, 2, 3},
                                                               {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
  }
  cout << sqrt(4.0 + (9. - 4.) / (3.0 - 2.0) * 0.3) << "\n";
  return 0;
}
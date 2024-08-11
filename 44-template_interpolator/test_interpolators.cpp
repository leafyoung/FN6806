#include <cmath>
#include <iostream>
using std::cout;
using std::sqrt;

#include "interpolator_inheritance.h"
#include "interpolator_template.h"
#include "interpolator_template_with_op.h"
#include "termstructure.h"

void test_interpolators() {
  {
    using namespace InterpolatorInheritance;
    cout << "InterpolatorInheritance with TermStructure:\n";
    TermStructureWithoutType<LinearInterpolator> linear({1, 2, 3}, {1, 2, 3});

    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructureWithoutType<SquaredInterpolator> squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
    cout << "\n";
  }
  {
    using namespace InterpolatorTemplate;
    cout << "InterpolatorTemplate with TermStructure:\n";
    TermStructure<LinearInterpolator, double, double> linear({1, 2, 3},
                                                             {1, 2, 3});

    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructure<SquaredInterpolator, double, double> squared({1, 2, 3},
                                                               {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
    cout << "\n";
  }
  {
    using namespace InterpolatorTemplate;
    cout << "InterpolatorTemplate with TermStructureWithoutType:\n";
    TermStructureWithoutType<LinearInterpolator<double, double>> linear(
        {1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructureWithoutType<SquaredInterpolator<double, double>> squared(
        {1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
    cout << "\n";
  }
  {
    using namespace InterpolatorTemplateWithOp;
    cout << "InterpolatorTemplateWithO with TermStructure:\n";
    TermStructure<LinearInterpolator, double, double> linear({1, 2, 3},
                                                             {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructure<SquaredInterpolator, double, double> squared({1, 2, 3},
                                                               {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
    cout << "\n";
  }
  {
    using namespace InterpolatorTemplateWithOp;
    cout << "InterpolatorTemplateWithOp with TermStructureWithoutType:\n";
    TermStructureWithoutType<LinearInterpolator<double, double>> linear(
        {1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << "\n";

    TermStructureWithoutType<SquaredInterpolator<double, double>> squared(
        {1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << "\n";
    cout << "\n";
  }
  cout << "Verification: \n";
  cout << sqrt(4.0 + (9. - 4.) / (3.0 - 2.0) * 0.3) << "\n\n";
}
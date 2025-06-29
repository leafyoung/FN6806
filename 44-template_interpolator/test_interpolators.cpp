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
    using YieldTermStructure = TermStructure<LinearInterpolator>;
    using VolatilityTermStructure = TermStructure<SquaredInterpolator>;

    cout << "InterpolatorInheritance with TermStructure:\n";
    YieldTermStructure linear({1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << '\n';

    VolatilityTermStructure squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << '\n';
    cout << '\n';
  }
  {
    using namespace InterpolatorTemplate;
    using YieldTermStructure =
        TermStructure<LinearInterpolator<double, double>>;
    using VolatilityTermStructure =
        TermStructure<SquaredInterpolator<double, double>>;

    cout << "InterpolatorTemplate with TermStructure:\n";
    YieldTermStructure linear({1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << '\n';

    VolatilityTermStructure squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << '\n';
    cout << '\n';
  }
  {
    using namespace InterpolatorTemplate;
    using YieldTermStructure =
        TermStructureWithType<LinearInterpolator, double, double>;
    using VolatilityTermStructure =
        TermStructureWithType<SquaredInterpolator, double, double>;

    cout << "InterpolatorTemplate with TermStructure:\n";
    YieldTermStructure linear({1, 2, 3}, {1, 2, 3});

    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << '\n';

    VolatilityTermStructure squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << '\n';
    cout << '\n';
  }
  {
    using namespace InterpolatorTemplateWithOp;
    using YieldTermStructure =
        TermStructureWithType<LinearInterpolator, double, double>;
    using VolatilityTermStructure =
        TermStructureWithType<SquaredInterpolator, double, double>;

    cout << "InterpolatorTemplateWithO with TermStructure:\n";
    YieldTermStructure linear({1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << '\n';

    VolatilityTermStructure squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << '\n';
    cout << '\n';
  }
  {
    using namespace InterpolatorTemplateWithOp;
    using YieldTermStructure =
        TermStructure<LinearInterpolator<double, double>>;
    using VolatilityTermStructure =
        TermStructure<SquaredInterpolator<double, double>>;

    cout << "InterpolatorTemplateWithOp with TermStructure:\n";
    YieldTermStructure linear({1, 2, 3}, {1, 2, 3});
    cout << linear.interpolate(0.5) << ", " << linear.interpolate(3.5) << ", "
         << linear.interpolate(2.3) << '\n';

    VolatilityTermStructure squared({1, 2, 3}, {1, 2, 3});
    cout << squared.interpolate(0.5) << ", " << squared.interpolate(3.5) << ", "
         << squared.interpolate(2.3) << '\n';
    cout << '\n';
  }
  cout << "Verification: \n";
  cout << sqrt(4.0 + (9. - 4.) / (3.0 - 2.0) * 0.3) << "\n\n";
}
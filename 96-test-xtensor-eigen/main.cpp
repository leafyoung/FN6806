#include <eigen3/Eigen/Dense>
#include <iostream>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include <iostream>

using namespace std;

struct X {
  struct {
    struct {
      int a;
    } Z;
  } Y;
};

enum PayoffType { CALL = 0, PUT = 1 };

enum OptionType { European = 0, American = 1 };

enum class PayoffTypeC { CALL = 0, PUT = 1 };

enum class OptionTypeC { European = 0, American = 1 };

int main() {
  // Misc test
  {
    int xx = 2147483648 - 1;
    cout << xx << '\n';

    int xx2 = 2147483648;  // implicit conversion from 'long' to 'int' changes value from 2147483648
                           // to -2147483648 [-Wimplicit-int-conversion]
    cout << xx2 << '\n';

    unsigned int xx3{2147483648u * 2 - 1};
    cout << xx3 << '\n';
    cout << (xx3 + 1) << '\n';

    cout << CALL << '\n';                // 0
    cout << (CALL == European) << '\n';  // 1 for true

    cout << static_cast<int>(PayoffTypeC::CALL) << '\n';  // 0
  }

  // test struct
  {
    X xx{{{3}}};
    // xx.Y.Z.a = 3;
    cout << ++xx.Y.Z.a << '\n';
  }
  // Test TypeId
  {
    auto vs = 1111111111111111111;
    cout << vs << '\n';
    cout << sizeof(vs) << '\n';
    cout << typeid(vs).name() << '\n';
  }

  // test eigen
  {
    cout << "test eigen" << '\n';

    using namespace Eigen;
    MatrixXd matr(2, 2);  // MatrixXd has doubles. MatrixXi has integers.
    matr(0, 0) = 1;
    matr(0, 1) = 2;
    matr(1, 0) = 3;
    matr(1, 1) = matr(1, 0) + matr(0, 0);

    Eigen::MatrixXd A(2, 2);
    A(0, 0) = 2.;
    A(1, 0) = -2.;
    A(0, 1) = 3.;
    A(1, 1) = 1.;

    Eigen::MatrixXd B(2, 3);
    B(0, 0) = 1.;
    B(1, 0) = 1.;
    B(0, 1) = 2.;
    B(1, 1) = 2.;
    B(0, 2) = -1.;
    B(1, 2) = 1.;

    auto C = A * B;

    auto D = B.cwiseProduct(C);
    std::cout << "coefficient-wise multiplication is:\n" << D << '\n';

    auto E = B + C;
    std::cout << "The sum of B & C is:\n" << E << '\n';

    std::cout << "The transpose of B is:\n" << B.transpose() << '\n';
    std::cout << "The A inverse is:\n" << A.inverse() << '\n';
    std::cout << "The determinant of A is:\n" << A.determinant() << '\n';

    auto my_func = [](double x) { return x * x; };
    std::cout << A.unaryExpr(my_func) << '\n';

    std::cout << matr << '\n' << '\n';

    MatrixXd matr2(2, 2);
    matr << 1, 2, 3, 4;
    std::cout << matr << '\n';
  }

  // test xtensor
  {
    xt::xarray<double> arr1{{1.0, 2.0, 3.0}, {2.0, 5.0, 7.0}, {2.0, 5.0, 7.0}};
    xt::xarray<double> arr2{5.0, 6.0, 7.0};
    xt::xarray<double> res = xt::view(arr1, 1) + arr2;
    std::cout << res << '\n';
  }
  cout << "end" << '\n';
  return 0;
}
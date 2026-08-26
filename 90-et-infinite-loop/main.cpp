// Below version comes from
// https://stackoverflow.com/questions/68007686/automatic-differentiation-using-expression-templates-c
// Final version (without expression template): https://github.com/dkaramit/sad

// Better version:
// https://github.com/dalon-work/AutoDiff/blob/master/src/ADBase.h

// Expression Template
// https://en.wikipedia.org/wiki/Expression_templates

#include <cmath>
#include <iostream>

// message to print during evaluation, in order to track the evaluation path.
#define msg2 std::cout << " msg: " << typeid(*this).name() << '\n';
#define msg ;

struct BaseExpression {
  BaseExpression() = default;
  virtual double evaluate() const = 0;
};

template <typename subExpr> struct GenericExpression : BaseExpression {
  const subExpr &self() const { return static_cast<const subExpr &>(*this); }
  subExpr &self() { return static_cast<subExpr &>(*this); }

  double evaluate() const {
    msg;
    return self().evaluate();
  };
};

class Number : public GenericExpression<Number> {
  double val;

public:
  Number() = default;

  Number(const double &x) : val(x) {}
  Number(const Number &x) : val(x.evaluate()) {}

  double evaluate() const {
    msg;
    return val;
  }
  double &evaluate() {
    msg;
    return val;
  }
};

template <typename leftHand, typename rightHand>
class Addition : public GenericExpression<Addition<leftHand, rightHand>> {
  const leftHand &LH;
  const rightHand &RH;

public:
  Addition(const leftHand &LH, const rightHand &RH) : LH(LH), RH(RH) {}

  double evaluate() const {
    msg;
    return LH.evaluate() + RH.evaluate();
  }
};

template <typename leftHand, typename rightHand>
Addition<leftHand, rightHand>
operator+(const GenericExpression<leftHand> &LH,
          const GenericExpression<rightHand> &RH) {
  return Addition<leftHand, rightHand>(LH.self(), RH.self());
}

class Expression : public GenericExpression<Expression> {
public:
  BaseExpression *baseExpr;

  Expression() = default;
  Expression(const Expression &E) { baseExpr = E.baseExpr; };
  // Expression(Expression *E){baseExpr = E->baseExpr;};

  double evaluate() const {
    msg;
    return baseExpr->evaluate();
  }

  template <typename subExpr>
  void assign(const GenericExpression<subExpr> &RH) {

    baseExpr = new subExpr(RH.self());
  }
};

using std::cout;
using '\n';

int main() {
  Number x(3.2);
  Number y(-2.3);
  Expression z, w;

  // works fine!
  z.assign(x);
  cout << z.evaluate() << endl;
  return 0;

  // works fine!
  z.assign(x + y);
  cout << z.evaluate() << endl;

  // works fine!
  w.assign(z + y);
  cout << w.evaluate() << endl;

  // Segmentation fault of z.evaluate() infinite recursion in between
  // LH.evaluate() (in Addition<Expression,Number>::evaluate()) and
  // baseExpr->evaluate() (in Expression::evaluate())
  z.assign(z + x);
  cout << z.evaluate() << endl;

  return 0;
}

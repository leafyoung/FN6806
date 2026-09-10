// https://github.com/leafyoung/FN6806/tree/main/40-inheritance_basic

#include "interpolator.h"
#include "zcb.h"

void test_override();
void test_override_virtual();
void test_ctor_dtor();
void test_delegation();
void test_zcb();
void test_interpolator();

/* ============================================== */

int main() {
  test_override();
  test_override_virtual();
  test_ctor_dtor();
  test_delegation();
  test_zcb();
  test_interpolator();
}
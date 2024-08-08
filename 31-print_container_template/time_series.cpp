#include "time_series.h"

void test_time_series() {
  auto t1 = TimeSeries<double>({1.1, 2.1, 3.1, 4.1});
  auto t2 = TimeSeries<int>({1, 2, 3, 4});
  t1.printData();
  t2.printData();
}
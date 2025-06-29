#pragma once

#include <initializer_list>
#include <iostream>
#include <vector>

using namespace std;

class TimeSeriesNT {
  vector<double> data;

public:
  // TimeSeriesNT(initializer_list<double> data) : data(data)
  // {}
  TimeSeriesNT(const vector<double> &data) : data(data) {}

  void printData() const {
    for (const auto &value : data) {
      cout << value << " ";
    }
    cout << '\n';
  }
};

template <typename T> class TimeSeries {
  vector<T> data;

public:
  // TimeSeries(initializer_list<T> data) : data(data) {}
  TimeSeries(const vector<T> &data) : data(data) {}

  void printData() const {
    for (const auto &value : data) {
      cout << value << ", ";
    }
    cout << '\n';
  }
};

void test_time_series();

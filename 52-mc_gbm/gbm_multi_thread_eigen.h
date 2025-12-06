#pragma once
#include "eigen3/Eigen/Dense"
#include "gbm_multi.h"
using namespace Eigen;

MatrixXd gbm_multipath_opt_thread_eigen(const GBMParam &gbm, const MCParam &mc,
                                        const Market &mkt, const Eval &eval,
                                        const int &n_thread);

// https://stackoverflow.com/questions/18400596/how-can-a-eigen-matrix-be-written-to-file-in-csv-format
#include <fstream>
#include <iostream>

// define the format you want, you only need one instance of this...
// see https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const static Eigen::IOFormat CSVFormat(Eigen::StreamPrecision,
                                       Eigen::DontAlignCols, ", ", "\n");

// writing functions taking Eigen types as parameters,
// see https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html
template <typename Derived>
void writeToCSVfile(std::string name,
                    const Eigen::MatrixBase<Derived> &matrix) {
  std::ofstream file(name.c_str());
  file << matrix.format(CSVFormat);
  // file.close() is not necessary,
  // desctructur closes file, see
  // https://en.cppreference.com/w/cpp/io/basic_ofstream
}

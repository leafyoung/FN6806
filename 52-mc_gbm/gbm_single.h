#pragma once
#include <random>
using namespace std;

using path = vector<double>;

path gbm_single_path_demo(double S0, double mu, double sigma, double T,
                          double dt, mt19937 &gen);

path gbm_single_path(double S0, double mu, double sigma, double T, double dt,
                     mt19937 &gen);

path gbm_single_path_v2(double S0, double mu, double sigma, double T, double dt,
                        mt19937 &gen);

path gbm_single_path_exp(double S0, double mu, double sigma, double T,
                         double dt, mt19937 &gen);

path gbm_single_path_exp_cumsum(double S0, double mu, double sigma, double T,
                                double dt, mt19937 &gen);

// european_option.cpp
#include "european_option.h"

#include <cmath>
#include <stdexcept>
#include <utility>

#include "logger.h"

namespace {

constexpr double kPi = 3.14159265358979323846;

struct BlackScholesTerms {
  double d1 = 0.0;
  double d2 = 0.0;
  double discount_factor = 0.0;
};

BlackScholesTerms make_terms(double spot, double strike, double rate, double vol, double maturity) {
  BlackScholesTerms terms;
  const double sqrt_maturity = std::sqrt(maturity);
  const double vol_sqrt_maturity = vol * sqrt_maturity;

  terms.d1 = (std::log(spot / strike) + (rate + 0.5 * vol * vol) * maturity) / vol_sqrt_maturity;
  terms.d2 = terms.d1 - vol_sqrt_maturity;
  terms.discount_factor = std::exp(-rate * maturity);
  return terms;
}

}  // namespace

// Standard normal PDF
double EuropeanOption::n(double x) {
  return std::exp(-0.5 * x * x) / std::sqrt(2.0 * kPi);
}

// Standard normal CDF using approximation
double EuropeanOption::N(double x) {
  const double a1 = 0.254829592;
  const double a2 = -0.284496736;
  const double a3 = 1.421413741;
  const double a4 = -1.453152027;
  const double a5 = 1.061405429;
  const double p = 0.3275911;

  const int sign = (x < 0.0) ? -1 : 1;
  x = std::fabs(x) / std::sqrt(2.0);

  const double t = 1.0 / (1.0 + p * x);
  const double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

  return 0.5 * (1.0 + sign * y);
}

EuropeanOption::EuropeanOption(std::string id, double spot, double strike, double rate, double vol,
                               double maturity, bool is_call)
    : Instrument(std::move(id)),
      spot_(spot),
      strike_(strike),
      rate_(rate),
      vol_(vol),
      T_(maturity),
      is_call_(is_call) {
  if (spot_ <= 0.0) {
    logging::error("EuropeanOption") << "event=option_construct status=failed reason=invalid_spot spot=" << spot_;
    throw std::invalid_argument("EuropeanOption: spot must be positive");
  }

  if (strike_ <= 0.0) {
    logging::error("EuropeanOption") << "event=option_construct status=failed reason=invalid_strike strike=" << strike_;
    throw std::invalid_argument("EuropeanOption: strike must be positive");
  }

  if (vol_ <= 0.0) {
    logging::error("EuropeanOption") << "event=option_construct status=failed reason=invalid_vol vol=" << vol_;
    throw std::invalid_argument("EuropeanOption: vol must be positive");
  }

  if (T_ <= 0.0) {
    logging::error("EuropeanOption") << "event=option_construct status=failed reason=invalid_maturity maturity=" << T_;
    throw std::invalid_argument("EuropeanOption: T must be positive");
  }

  const double moneyness = spot_ / strike_;
  const std::string moneyness_label =
      (moneyness > 1.0) ? "ITM" : (moneyness < 1.0) ? "OTM" : "ATM";

  logging::info("EuropeanOption") << "event=option_construct status=success instrument_id=" << this->id()
                                                                                        << " spot=" << spot_
                                                                                        << " strike=" << strike_
                                                                                        << " rate=" << rate_
                                                                                        << " vol=" << vol_
                                                                                        << " maturity=" << T_
                                                                                        << " option_type="
                                                                                        << (is_call_ ? "call"
                                                                                                     : "put")
                                                                                        << " moneyness="
                                                                                        << moneyness_label;
}

double EuropeanOption::price(double rate) const {
  const BlackScholesTerms terms = make_terms(spot_, strike_, rate, vol_, T_);

  const double pv = is_call_ ? spot_ * N(terms.d1) - strike_ * terms.discount_factor * N(terms.d2)
                             : strike_ * terms.discount_factor * N(-terms.d2) - spot_ * N(-terms.d1);

  logging::debug("EuropeanOption") << "event=option_price status=success instrument_id=" << id()
                                                                                    << " rate=" << rate
                                                                                    << " pv=" << pv;
  return pv;
}

double EuropeanOption::duration(double rate) const {
  const BlackScholesTerms terms = make_terms(spot_, strike_, rate, vol_, T_);

  const double rho = is_call_ ? strike_ * T_ * terms.discount_factor * N(terms.d2)
                              : -strike_ * T_ * terms.discount_factor * N(-terms.d2);

  logging::debug("EuropeanOption") << "event=option_rho status=success instrument_id=" << id()
                                                                                  << " rate=" << rate
                                                                                  << " rho=" << rho;
  return rho;
}

double EuropeanOption::delta() const {
  const BlackScholesTerms terms = make_terms(spot_, strike_, rate_, vol_, T_);
  const double delta = is_call_ ? N(terms.d1) : N(terms.d1) - 1.0;

  logging::debug("EuropeanOption") << "event=option_delta status=success instrument_id=" << id()
                                                                                    << " delta=" << delta;
  return delta;
}

double EuropeanOption::vega() const {
  const BlackScholesTerms terms = make_terms(spot_, strike_, rate_, vol_, T_);
  const double vega = spot_ * std::sqrt(T_) * n(terms.d1);

  logging::debug("EuropeanOption") << "event=option_vega status=success instrument_id=" << id()
                                                                                   << " vega=" << vega;
  return vega;
}

double EuropeanOption::gamma() const {
  const BlackScholesTerms terms = make_terms(spot_, strike_, rate_, vol_, T_);
  const double gamma = n(terms.d1) / (spot_ * vol_ * std::sqrt(T_));

  logging::debug("EuropeanOption") << "event=option_gamma status=success instrument_id=" << id()
                                                                                    << " gamma=" << gamma;
  return gamma;
}

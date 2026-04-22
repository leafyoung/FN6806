// instrument_csv.cpp
#include "instrument_csv.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "european_option.h"
#include "fixed_rate_bond.h"
#include "logger.h"

namespace instrument_csv {
namespace {

std::string trim(std::string value) {
  auto is_space = [](unsigned char ch) { return std::isspace(ch) != 0; };

  auto begin = value.begin();
  while (begin != value.end() && is_space(static_cast<unsigned char>(*begin))) {
    ++begin;
  }

  auto end = value.end();
  while (end != begin && is_space(static_cast<unsigned char>(*(end - 1)))) {
    --end;
  }

  return std::string(begin, end);
}

std::vector<std::string> split_csv(const std::string& line) {
  std::vector<std::string> parts;
  std::istringstream input(line);
  std::string part;

  while (std::getline(input, part, ',')) {
    parts.push_back(trim(part));
  }

  return parts;
}

double parse_double(const std::string& value, const std::string& field_name) {
  try {
    return std::stod(value);
  } catch (...) {
    throw std::invalid_argument("cannot parse " + field_name + ": " + value);
  }
}

int parse_int(const std::string& value, const std::string& field_name) {
  try {
    return std::stoi(value);
  } catch (...) {
    throw std::invalid_argument("cannot parse " + field_name + ": " + value);
  }
}

bool parse_bool_token(const std::string& value) {
  return value == "call" || value == "1" || value == "true" || value == "True";
}

struct InstrumentRow {
  std::string id;
  std::string type;
  std::vector<std::string> fields;

  static InstrumentRow from_line(const std::string& line) {
    InstrumentRow row;
    row.fields = split_csv(line);

    if (row.fields.size() < 2) {
      throw std::invalid_argument("instrument row requires at least id and type: " + line);
    }

    row.id = row.fields[0];
    row.type = row.fields[1];
    return row;
  }
};

}  // namespace

std::unique_ptr<Instrument> parse_instrument_line(const std::string& line) {
  const InstrumentRow row = InstrumentRow::from_line(line);

  if (row.type == "bond" || row.type == "FixedRateBond") {
    if (row.fields.size() < 5) {
      throw std::invalid_argument("bond requires 5 fields: " + line);
    }

    const double face = parse_double(row.fields[2], "face");
    const double coupon = parse_double(row.fields[3], "coupon");
    const int maturity = parse_int(row.fields[4], "maturity");

    logging::debug("InstrumentCsv")
        << "event=instrument_parse status=success type=bond instrument_id=" << row.id
        << " face=" << face << " coupon=" << coupon << " maturity=" << maturity;

    return std::make_unique<FixedRateBond>(row.id, face, coupon, maturity);
  }

  if (row.type == "option" || row.type == "EuropeanOption") {
    if (row.fields.size() < 7) {
      throw std::invalid_argument("option requires 7 fields: " + line);
    }

    const double spot = parse_double(row.fields[2], "spot");
    const double strike = parse_double(row.fields[3], "strike");
    const double maturity = parse_double(row.fields[4], "maturity");
    const double vol = parse_double(row.fields[5], "vol");
    const bool is_call = parse_bool_token(row.fields[6]);

    logging::debug("InstrumentCsv")
        << "event=instrument_parse status=success type=option instrument_id=" << row.id
        << " spot=" << spot << " strike=" << strike << " vol=" << vol << " maturity=" << maturity
        << " option_type=" << (is_call ? "call" : "put");

    return std::make_unique<EuropeanOption>(row.id, spot, strike, vol, maturity, is_call);
  }

  throw std::invalid_argument("unknown instrument type: " + row.type);
}

}  // namespace instrument_csv

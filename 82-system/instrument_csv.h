// instrument_csv.h
#pragma once

#include <memory>
#include <string>

class Instrument;

namespace instrument_csv {

std::unique_ptr<Instrument> parse_instrument_line(const std::string& line);

}  // namespace instrument_csv

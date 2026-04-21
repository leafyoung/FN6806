// risk_report.cpp
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "csv_writer.h"
#include "logger.h"
#include "portfolio.h"
#include "risk_report.h"

void generate_risk_report(const Portfolio& portfolio,
                          double base_rate,
                          const std::string& output_path) {
  logging::info("RiskReport") << "event=risk_report status=start instrument_count=" << portfolio.size()
                            << " base_rate=" << base_rate << " output_path=" << output_path;

  CSVWriter writer(output_path);  // throws if file cannot be opened
  writer.write("id,type,pv,dv01,pv_up100bps,pv_down100bps");

  const double shifted_up = base_rate + 0.01;   // +100 bps
  const double shifted_dn = base_rate - 0.01;   // -100 bps

  int count = 0;
  portfolio.for_each([&](const Instrument& instr) {
    ++count;

    logging::debug("RiskReport") << "event=risk_report_row status=pricing row_index=" << count
                             << " instrument_count=" << portfolio.size()
                             << " instrument_id=" << instr.id();

    const double pv   = instr.price(base_rate);
    const double dv01 = instr.dv01(base_rate);

    // Stress test: price at shifted rates (no mutation of shared state)
    logging::debug("RiskReport") << "event=risk_report_row status=stress_test instrument_id="
                             << instr.id() << " shift_bps=100";
    const double pv_up = instr.price(shifted_up);
    const double pv_dn = instr.price(shifted_dn);

    std::ostringstream row;
    row << instr.id() << ','
        << instr.type_name() << ','
        << std::fixed << std::setprecision(4)
        << pv << ',' << dv01 << ','
        << pv_up << ',' << pv_dn;
    writer.write(row.str());

    logging::debug("RiskReport") << "event=risk_report_row status=written instrument_id="
                             << instr.id() << " row=" << row.str();
  });

  logging::info("RiskReport") << "event=risk_report status=complete written_count=" << count
                            << " output_path=" << output_path;
  std::cout << "Risk report written to " << output_path << '\n';
}

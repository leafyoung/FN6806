// risk_report.cpp
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "csv_writer.h"
#include "logger.h"
#include "portfolio.h"
#include "risk_report.h"
#include "yield_curve.h"

void generate_risk_report(const Portfolio& portfolio, const YieldCurve& curve,
                          const std::string& output_path, double scenario_shift_bp) {
  logging::info("RiskReport") << "event=risk_report status=start instrument_count="
                              << portfolio.size() << " output_path=" << output_path;

  CSVWriter writer(output_path);  // throws if file cannot be opened
  const int shift_bps_label = static_cast<int>(scenario_shift_bp);
  writer.write("id,type,pv,dv01,pv_up" + std::to_string(shift_bps_label) + "bps,pv_down" +
               std::to_string(shift_bps_label) + "bps");
  YieldCurve shifted_up_curve = curve;
  YieldCurve shifted_down_curve = curve;
  shifted_up_curve.parallel_shift(scenario_shift_bp / 10000.0);
  shifted_down_curve.parallel_shift(-scenario_shift_bp / 10000.0);

  int count = 0;
  portfolio.for_each([&](const Instrument& instr) {
    ++count;

    logging::debug("RiskReport") << "event=risk_report_row status=pricing row_index=" << count
                                 << " instrument_count=" << portfolio.size()
                                 << " instrument_id=" << instr.id();

    const double pv = instr.price(curve);
    const double dv01 = instr.dv01(curve);

    logging::debug("RiskReport") << "event=risk_report_row status=stress_test instrument_id="
                                 << instr.id() << " shift_bps=" << (scenario_shift_bp);
    const double pv_up = instr.price(shifted_up_curve);
    const double pv_dn = instr.price(shifted_down_curve);

    std::ostringstream row;
    row << instr.id() << ',' << instr.type_name() << ',' << std::fixed << std::setprecision(4) << pv
        << ',' << dv01 << ',' << pv_up << ',' << pv_dn;
    writer.write(row.str());

    logging::debug("RiskReport") << "event=risk_report_row status=written instrument_id="
                                 << instr.id() << " row=" << row.str();
  });

  logging::info("RiskReport") << "event=risk_report status=complete written_count=" << count
                              << " output_path=" << output_path;
  std::cout << "Risk report written to " << output_path << '\n';
}

#include <iomanip>
#include <iostream>
#include <string>

#include "csv_writer.h"
#include "risk_report.h"

void generate_risk_report(const Portfolio &portfolio,
                          std::shared_ptr<YieldCurve> curve,
                          const std::string &output_path) {
  CSVWriter writer(output_path); // throws if file cannot be opened
  writer.write("id,type,pv,dv01,pv_up100bps,pv_down100bps");

  const double base_rate = curve->rate_at(5.0);

  for (const auto &instr : portfolio) {
    const double pv = instr->price(base_rate);
    const double dv01 = instr->dv01(base_rate);

    // Stress test: parallel shift ±100 bps
    { // RAII-style scope: always restore even if pricing throws
      auto restore = [&curve, shift = 0.0](double delta) mutable {
        shift += delta;
        curve->parallel_shift(delta);
      };
      restore(+0.01);
      const double pv_up = instr->price(curve->rate_at(5.0));
      restore(-0.02);
      const double pv_dn = instr->price(curve->rate_at(5.0));
      restore(+0.01); // restore

      std::ostringstream row;
      row << instr->id() << ',' << instr->type_name() << ',' << std::fixed
          << std::setprecision(4) << pv << ',' << dv01 << ',' << pv_up << ','
          << pv_dn;
      writer.write(row.str());
    }
  }
  std::cout << "Risk report written to " << output_path << '\n';
}
// https://replit.com/@YeKunlun/70-chrono?v=1
// With --std=c++2b

#include "date.h"
#include "tz.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;

void test_ymd() {
  cout << __func__ << "\n";

  const auto now = std::chrono::system_clock::now();

  const std::chrono::year_month_day ymd_now{
      std::chrono::floor<std::chrono::days>(now)};

  cout << static_cast<int>(ymd_now.year()) << "/"
            << static_cast<unsigned>(ymd_now.month()) << "/"
            << static_cast<unsigned>(ymd_now.day()) << "\n";

  std::chrono::year_month_day ymd{std::chrono::year{2002},
                                  std::chrono::month{11}, std::chrono::day{14}};

  ymd = std::chrono::year{2002} / std::chrono::month{11} / std::chrono::day{14};

  auto date1 = std::chrono::year{2002} / 11 / 14;

  auto the_year = static_cast<int>(date1.year());
  auto the_month = static_cast<unsigned>(date1.month());
  auto the_day = static_cast<unsigned>(date1.day());

  cout << the_year << "/" << the_month << "/" << the_day << "\n";
}

void test_duration() {
  cout << __func__ << "\n";
  using namespace std::chrono_literals;
  using namespace std::chrono;

  const auto start = std::chrono::steady_clock::now();
  std::this_thread::sleep_for(2s);
  const auto end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds = end - start;

  cout << elapsed_seconds.count()
            << '\n'; // C++20: operator<< chrono::duration

  auto d = 5min + 10s;
  cout << duration_cast<seconds>(d).count() << "\n";
  cout << duration_cast<milliseconds>(d).count() << "\n";
  cout << duration_cast<days>(d).count() << "\n";
}

void test_weekday() {
  cout << __func__ << "\n";

  using namespace std::chrono;

  weekday{month{6} / 21 / 2016};
  cout << weekday{June / 21 / 2016}.c_encoding() << "\n";
  cout << weekday{June / 21 / 2016}.iso_encoding() << "\n";
  // cout << weekday{June / 21 / 2016} << "\\n";
  static_assert(weekday{June / 21 / 2016} == Tuesday);

  constexpr auto second_tuesday_in_October_2019 =
      year_month_day{Tuesday[2] / October / 2019y};

  constexpr auto last_tuesday_in_October_2019 =
      year_month_day{Tuesday[last] / October / 2019y};

  // cout << second_tuesday_in_October_2019<< '\n'
  //          << last_tuesday_in_October_2019 << '\n';
}

void test_date_h() {
  cout << __func__ << "\n";
  using namespace date;

  auto d2 = 2015_y / aug / 31;
  auto d3 = d2 + months(1) - years(1);
  cout << "d2:" << d2 << "\n";
  cout << "d3:" << d3 << "\n";
 
  // sys_days normalizes an ymd and it allows +/- days()
  cout << "d3:" << sys_days(d3) << "\n";
  auto d4 = sys_days(d3) + days(100);
  cout << "d4:" << d4 << "\n";

  cout << "d3 ops:" << (sys_days(d3) + days(1) - sys_days(d3)).count()
            << "\n";

  cout << "year(2015):" << year(2015) / sep / 31 << "\n";

  cout << "weekday:" << weekday{year(2015) / sep / 31} << "\n";
  cout << "weekday2:" << (weekday{year(2015) / sep / 31} == Thursday)
            << "\n";

  cout << "month(7):" << month(7) / day(4) / year(1994) << "\n";

  cout << "24h:" << (4 * 24h) + 4h + 33min + 3s << "\n";
}

void test_date_adv() {
  cout << __func__ << "\n";

  using namespace date;
  using day_point = std::chrono::time_point<std::chrono::system_clock, days>;
  using time_point =
      std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

  auto d2 = 2015_y / aug / 5;
  auto d3 = d2 - months(3) - years(1);

  cout << (floor<days>(day_point{d2} - day_point{d3}) + +days{100}).count()
            << "\n";

  cout << year(2014) / 2 / 30 << "\\n";
  cout << typeid(year(2014) / 2 / 30).name() << "\\n";
  year_month_day x;
  cout << typeid(x).name() << "\\n";

  day_point dp = floor<days>(std::chrono::system_clock::now());
  cout << dp.time_since_epoch().count() << "\n";

  cout << day_point{jan / 1 / 2000} - day_point{jan / 1 / 1999} << "\n";

  cout << day_point{jan / 1 / 2000} - day_point{jan / 1 / 1999} << "\n";

  cout << day_point{jan / 1 / 2000}.time_since_epoch() -
                   day_point{jan / 1 / 1999}.time_since_epoch()
            << "\n";

  auto tp = day_point{jan / 1 / 2000} + 144h + 33min + 59s;
  cout << tp << "\n";
  auto d = floor<days>(tp);
  cout << d << "\n";

  auto time = make_time(4h + 33min + 3s);
  cout << time << "\n" << time.seconds() << "\n";
}

int main() {
  test_date_h();
  cout << "\n";
  test_date_adv();
  cout << "\n";
  // test_ymd();
  // cout << "\n";
  // test_duration();
  // cout << "\n";
}
#include <cfloat>
#include <cmath>
#include <limits>
#include <memory>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  const auto load = std::make_shared<VerticalEccentric>()
                        ->f<"Set"_f, "V"_f>(100000.)
                        ->f<"Set"_f, "E"_f>(0.25);
  const auto foundation =
      std::make_shared<FoundationStrip>()->f<"Set"_f, "B"_f>(1.);
  const auto calc = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load, foundation);

  JTEST_EQ((calc->f<"Get"_f, "B_"_f>()), 0.5);
  JTEST_EQ((calc->f<"Get"_f, "Qref"_f>()), 200000.);
  const auto load2 = load->f<"Set"_f, "E"_f>(0.5);
  const auto calc2 = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load2, foundation);
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<"Get"_f, "Qref"_f>()));
  const auto calc3 = calc2->f<"Set"_f, "Clone"_f>();
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), (calc3->f<"Get"_f, "B_"_f>()));
  JTEST_EQ((calc2->f<"Get"_f, "Qref"_f>()), (calc3->f<"Get"_f, "Qref"_f>()));

  const auto load_maxe = load->f<"Set"_f, "E"_f>(DBL_MAX);
  const auto calc_maxe = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load_maxe, foundation);
  JTEST_EQ((calc_maxe->f<"Get"_f, "B_"_f>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<"Set"_f, "E"_f>(-DBL_MAX);
  const auto calc_mine = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load_mine, foundation);
  JTEST_EQ((calc_mine->f<"Get"_f, "B_"_f>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofAllDecorator)  // NOLINT
{
  using DecoratorFoundation =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<FoundationStrip>>>>;
  using DecoratorLoad =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<VerticalEccentric>>>>;
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<DecoratorLoad, DecoratorFoundation>>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<DecoratorFoundation>(log, log);
  const auto foundation2 = foundation->f<"Set"_f, "B"_f>(1.);
  const auto load_1 = std::make_shared<DecoratorLoad>(log, log);
  const auto load_2 = load_1->f<"Set"_f, "V"_f>(100000.);
  const auto load = load_2->f<"Set"_f, "E"_f>(0.25);
  const auto calc =
      std::make_shared<DecoratorCalc>(log, log, load, foundation2);
  JTEST_EQ((calc->f<"Get"_f, "B_"_f>()), 0.5);
  JTEST_EQ((calc->f<"Get"_f, "Qref"_f>()), 200000.);
  const auto load2 = load->f<"Set"_f, "E"_f>(0.5);
  const auto calc2 =
      std::make_shared<DecoratorCalc>(log, log, load2, foundation2);
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<"Get"_f, "Qref"_f>()));
  const auto calc3 = calc2->f<"Set"_f, "Clone"_f>();
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), (calc3->f<"Get"_f, "B_"_f>()));
  JTEST_EQ((calc2->f<"Get"_f, "Qref"_f>()), (calc3->f<"Get"_f, "Qref"_f>()));

  const auto load_maxe = load->f<"Set"_f, "E"_f>(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(log, log, load_maxe, foundation2);
  JTEST_EQ((calc_maxe->f<"Get"_f, "B_"_f>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<"Set"_f, "E"_f>(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(log, log, load_mine, foundation2);
  JTEST_EQ((calc_mine->f<"Get"_f, "B_"_f>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofMainDecorator)  // NOLINT
{
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<FoundationStrip>();
  const auto foundation2 = foundation->f<"Set"_f, "B"_f>(1.);
  const auto load_1 = std::make_shared<VerticalEccentric>();
  const auto load_2 = load_1->f<"Set"_f, "V"_f>(100000.);
  const auto load = load_2->f<"Set"_f, "E"_f>(0.25);
  const auto calc =
      std::make_shared<DecoratorCalc>(log, log, load, foundation2);
  JTEST_EQ((calc->f<"Get"_f, "B_"_f>()), 0.5);
  JTEST_EQ((calc->f<"Get"_f, "Qref"_f>()), 200000.);
  const auto load2 = load->f<"Set"_f, "E"_f>(0.5);
  const auto calc2 =
      std::make_shared<DecoratorCalc>(log, log, load2, foundation2);
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<"Get"_f, "Qref"_f>()));
  const auto calc3 = calc2->f<"Set"_f, "Clone"_f>();
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), (calc3->f<"Get"_f, "B_"_f>()));
  JTEST_EQ((calc2->f<"Get"_f, "Qref"_f>()), (calc3->f<"Get"_f, "Qref"_f>()));

  const auto load_maxe = load->f<"Set"_f, "E"_f>(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(log, log, load_maxe, foundation2);
  JTEST_EQ((calc_maxe->f<"Get"_f, "B_"_f>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<"Set"_f, "E"_f>(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(log, log, load_mine, foundation2);
  JTEST_EQ((calc_mine->f<"Get"_f, "B_"_f>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofOnly)  // NOLINT
{
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  auto log = spdlog::get("log");

  const auto calc = std::make_shared<DecoratorCalc>(
      log, log, std::make_shared<VerticalEccentric>(),
      std::make_shared<FoundationStrip>());
  const auto calc2 = calc->f<"Set"_f, "Load"_f, "E"_f>(0.25)
                         ->f<"Set"_f, "Load"_f, "V"_f>(100000.)
                         ->f<"Set"_f, "_foundation"_f, "B"_f>(1.);
  JTEST_EQ((calc2->f<"Get"_f, "Load"_f, "E"_f>()), 0.25);
  JTEST_EQ((calc2->f<"Get"_f, "Load"_f, "V"_f>()), 100000.);
  JTEST_EQ((calc2->f<"Get"_f, "_foundation"_f, "B"_f>()), 1.);
  JTEST_EQ((calc2->f<"Get"_f, "B_"_f>()), 0.5);
  JTEST_EQ((calc2->f<"Get"_f, "Qref"_f>()), 200000.);
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

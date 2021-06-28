#include <cmath>
#include <memory>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, VerticalEccentric)  // NOLINT
{
  const auto load = std::make_shared<VerticalEccentric>();
  JTEST_TRUE(std::isnan(load->f<F::Get, F::E>()));
  JTEST_TRUE(std::isnan(load->f<F::Get, F::V>()));
  const auto load2 = load->f<F::Set, F::V>(100000.);
  JTEST_TRUE(std::isnan(load->f<F::Get, F::E>()));
  JTEST_EQ((load2->f<F::Get, F::V>()), 100000.);
  const auto load3 = load2->f<F::Set, F::E>(0.2);
  JTEST_EQ((load3->f<F::Get, F::E>()), 0.2);
  JTEST_EQ((load3->f<F::Get, F::V>()), 100000.);
  const auto load4 = load3->f<F::Set, F::Clone>();
  JTEST_EQ((load3->f<F::Get, F::E>()), (load4->f<F::Get, F::E>()));
  JTEST_EQ((load3->f<F::Get, F::V>()), (load4->f<F::Get, F::V>()));
}

JTEST_NAME(data, VerticalEccentricDecorator)  // NOLINT
{
  using Decorator =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<VerticalEccentric>>>>;

  auto log = spdlog::get("log");

  const auto load = std::make_shared<Decorator>(log, log);
  JTEST_TRUE(std::isnan(load->f<F::Get, F::E>()));
  JTEST_TRUE(std::isnan(load->f<F::Get, F::V>()));
  const auto load2 = load->f<F::Set, F::V>(100000.);
  JTEST_TRUE(std::isnan(load->f<F::Get, F::E>()));
  JTEST_EQ((load2->f<F::Get, F::V>()), 100000.);
  const auto load3 = load2->f<F::Set, F::E>(0.2);
  JTEST_EQ((load3->f<F::Get, F::E>()), 0.2);
  JTEST_EQ((load3->f<F::Get, F::V>()), 100000.);
  const auto load4 = load3->f<F::Set, F::Clone>();
  JTEST_EQ((load3->f<F::Get, F::E>()), (load4->f<F::Get, F::E>()));
  JTEST_EQ((load3->f<F::Get, F::V>()), (load4->f<F::Get, F::V>()));
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

#include <cmath>
#include <memory>
#include <utility>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/data/load/decorator_vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, VerticalEccentric)  // NOLINT
{
  const auto load = std::make_shared<VerticalEccentric>();
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_TRUE(std::isnan(load->V()));
  const auto load2 = load->SetV(100000.);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_EQ(load2->V(), 100000.);
  const auto load3 = load2->SetE(0.2);
  JTEST_EQ(load3->E(), 0.2);
  JTEST_EQ(load3->V(), 100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ(load3->E(), load4->E());
  JTEST_EQ(load3->V(), load4->V());
}

JTEST_NAME(data, VerticalEccentricDecorator)  // NOLINT
{
  using Decorator = DecoratorStartVerticalEccentric<
      LogCall<LogDuration<DecoratorEndVerticalEccentric<VerticalEccentric>>>>;

  auto log = spdlog::get("log");

  const auto load = std::make_shared<Decorator>(log, log);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_TRUE(std::isnan(load->V()));
  const auto load2 = load->SetV(100000.);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_EQ(load2->V(), 100000.);
  const auto load3 = load2->SetE(0.2);
  JTEST_EQ(load3->E(), 0.2);
  JTEST_EQ(load3->V(), 100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ(load3->E(), load4->E());
  JTEST_EQ(load3->V(), load4->V());
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

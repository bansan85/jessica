#include <cmath>
#include <memory>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/helper/accessor.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, FoundationStrip)  // NOLINT
{
  const auto foundation = std::make_shared<FoundationStrip>();
  JTEST_TRUE(std::isnan(foundation->f<"Get"_f, "B"_f>()));
  const auto foundation2 = foundation->f<"Set"_f, "B"_f>(1.);
  JTEST_TRUE(std::isnan(foundation->f<"Get"_f, "B"_f>()));
  JTEST_EQ((foundation2->f<"Get"_f, "B"_f>()), 1.);
  const auto foundation3 = foundation2->f<"Set"_f, "Clone"_f>();
  JTEST_EQ((foundation2->f<"Get"_f, "B"_f>()),
           (foundation3->f<"Get"_f, "B"_f>()));
}

JTEST_NAME(data, FoundationStripDecorator)  // NOLINT
{
  using Decorator =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<FoundationStrip>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<Decorator>(log, log);
  JTEST_TRUE(std::isnan(foundation->f<"Get"_f, "B"_f>()));
  const auto foundation2 = foundation->f<"Set"_f, "B"_f>(1.);
  JTEST_TRUE(std::isnan(foundation->f<"Get"_f, "B"_f>()));
  JTEST_EQ((foundation2->f<"Get"_f, "B"_f>()), 1.);
  const auto foundation3 = foundation2->f<"Set"_f, "Clone"_f>();
  JTEST_EQ((foundation2->f<"Get"_f, "B"_f>()),
           (foundation3->f<"Get"_f, "B"_f>()));
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

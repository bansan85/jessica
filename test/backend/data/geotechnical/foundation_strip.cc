#include <cmath>
#include <memory>

#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

using namespace jessica;

JTEST_NAME(data, FoundationStrip)  // NOLINT
{
  const auto foundation =
      std::make_shared<FoundationStrip<FoundationStripImpl>>();
  JTEST_TRUE(std::isnan(foundation->f<F::Get, F::B>()));
  const auto foundation2 = foundation->f<F::Set, F::B>(1.);
  JTEST_TRUE(std::isnan(foundation->f<F::Get, F::B>()));
  JTEST_EQ((foundation2->f<F::Get, F::B>()), 1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ((foundation2->f<F::Get, F::B>()), (foundation3->f<F::Get, F::B>()));
}

JTEST_NAME(data, FoundationStripDecorator)  // NOLINT
{
  using Decorator = LogCall<LogDuration<FoundationStripImpl>>;

  const auto foundation = std::make_shared<FoundationStrip<Decorator>>();
  JTEST_TRUE(std::isnan(foundation->f<F::Get, F::B>()));
  const auto foundation2 = foundation->f<F::Set, F::B>(1.);
  JTEST_TRUE(std::isnan(foundation->f<F::Get, F::B>()));
  JTEST_EQ((foundation2->f<F::Get, F::B>()), 1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ((foundation2->f<F::Get, F::B>()), (foundation3->f<F::Get, F::B>()));
}

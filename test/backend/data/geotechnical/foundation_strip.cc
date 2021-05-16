#include <cmath>
#include <memory>

#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

JTEST_NAME(data, FoundationStrip)  // NOLINT
{
  const auto foundation =
      std::make_shared<Jessica::Data::Geotechnical::FoundationStrip<
          Jessica::Data::Geotechnical::FoundationStripImpl>>();
  JTEST_TRUE(std::isnan(
      foundation
          ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
  const auto foundation2 =
      foundation->f<true, Jessica::Data::Geotechnical::FoundationStripImpl::B>(
          1.);
  JTEST_TRUE(std::isnan(
      foundation
          ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
  JTEST_EQ(
      (foundation2
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()),
      1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ(
      (foundation2
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()),
      (foundation3
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
}

JTEST_NAME(data, FoundationStripDecorator)  // NOLINT
{
  using Decorator =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Data::Geotechnical::FoundationStripImpl>>;

  const auto foundation = std::make_shared<
      Jessica::Data::Geotechnical::FoundationStrip<Decorator>>();
  JTEST_TRUE(std::isnan(
      foundation
          ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
  const auto foundation2 =
      foundation->f<true, Jessica::Data::Geotechnical::FoundationStripImpl::B>(
          1.);
  JTEST_TRUE(std::isnan(
      foundation
          ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
  JTEST_EQ(
      (foundation2
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()),
      1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ(
      (foundation2
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()),
      (foundation3
           ->f<false, Jessica::Data::Geotechnical::FoundationStripImpl::B>()));
}

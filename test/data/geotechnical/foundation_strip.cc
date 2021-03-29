#include <cmath>
#include <memory>

#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/test/test.h>

JTEST_NAME(data, FoundationStrip)  // NOLINT
{
  const auto foundation = Data::Geotechnical::Create::FoundationStrip();
  JTEST_TRUE(std::isnan(foundation->B()));
  const auto foundation2 = foundation->B(1.);
  JTEST_TRUE(std::isnan(foundation->B()));
  JTEST_EQ(foundation2->B(), 1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ(foundation2->B(), foundation3->B());
}

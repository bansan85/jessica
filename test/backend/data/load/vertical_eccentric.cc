#include <cmath>
#include <memory>

#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

JTEST_NAME(data, VerticalEccentric)  // NOLINT
{
  const auto load = std::make_shared<Jessica::Data::Load::VerticalEccentric<
      Jessica::Data::Load::VerticalEccentricImpl>>();
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()));
  const auto load2 =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::V>(100000.);
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_EQ((load2->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           100000.);
  const auto load3 =
      load2->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.2);
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()),
           0.2);
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()),
           (load4->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           (load4->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()));
}

JTEST_NAME(data, VerticalEccentricDecorator)  // NOLINT
{
  using Decorator =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Data::Load::VerticalEccentricImpl>>;

  const auto load =
      std::make_shared<Jessica::Data::Load::VerticalEccentric<Decorator>>();
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()));
  const auto load2 =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::V>(100000.);
  JTEST_TRUE(std::isnan(
      load->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_EQ((load2->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           100000.);
  const auto load3 =
      load2->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.2);
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()),
           0.2);
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()),
           (load4->f<false, Jessica::Data::Load::VerticalEccentricImpl::E>()));
  JTEST_EQ((load3->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()),
           (load4->f<false, Jessica::Data::Load::VerticalEccentricImpl::V>()));
}

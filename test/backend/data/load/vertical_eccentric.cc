#include <cmath>
#include <memory>

#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

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
  using Decorator = VerticalEccentricDecorator<LogCall<
      LogDuration<VerticalEccentric, VerticalEccentric>, VerticalEccentric>>;

  const auto load = std::make_shared<Decorator>();
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

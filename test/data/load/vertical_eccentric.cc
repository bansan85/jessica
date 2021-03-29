#include <cmath>
#include <memory>

#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>

JTEST_NAME(data, VerticalEccentric)  // NOLINT
{
  const auto load = Data::Load::Create::VerticalEccentric();
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_TRUE(std::isnan(load->V()));
  const auto load2 = load->V(100000.);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_EQ(load2->V(), 100000.);
  const auto load3 = load2->E(0.2);
  JTEST_EQ(load3->E(), 0.2);
  JTEST_EQ(load3->V(), 100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ(load3->E(), load4->E());
  JTEST_EQ(load3->V(), load4->V());
}

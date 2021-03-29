#include <cmath>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  const auto foundation = Data::Geotechnical::Create::FoundationStrip()->B(1.);
  const auto load =
      Data::Load::Create::VerticalEccentric()->V(100000.)->E(0.25);
  const auto calc =
      Calc::Geotechnical::Create::MeyerhofShallowFoundation(load, foundation);
  JTEST_EQ(calc->B_(), 0.5);
  JTEST_EQ(calc->Qref(), 200000.);
  const auto load2 = load->E(0.5);
  const auto calc2 =
      Calc::Geotechnical::Create::MeyerhofShallowFoundation(load2, foundation);
  JTEST_EQ(calc2->B_(), 0.);
  JTEST_TRUE(std::isinf(calc2->Qref()));
}

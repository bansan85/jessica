#include <cfloat>
#include <cmath>
#include <limits>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  const auto foundation =
      Jessica::Data::Geotechnical::Create::FoundationStrip()->B(1.);
  const auto load =
      Jessica::Data::Load::Create::VerticalEccentric()->V(100000.)->E(0.25);
  const auto calc =
      Jessica::Calc::Geotechnical::Create::MeyerhofShallowFoundation(
          load, foundation);
  JTEST_EQ(calc->B_(), 0.5);
  JTEST_EQ(calc->Qref(), 200000.);
  const auto load2 = load->E(0.5);
  const auto calc2 =
      Jessica::Calc::Geotechnical::Create::MeyerhofShallowFoundation(
          load2, foundation);
  JTEST_EQ(calc2->B_(), 0.);
  JTEST_TRUE(std::isinf(calc2->Qref()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ(calc2->B_(), calc3->B_());
  JTEST_EQ(calc2->Qref(), calc3->Qref());

  const auto load_maxe = load->E(DBL_MAX);
  const auto calc_maxe =
      Jessica::Calc::Geotechnical::Create::MeyerhofShallowFoundation(
          load_maxe, foundation);
  JTEST_EQ(calc_maxe->B_(), -std::numeric_limits<double>::infinity());

  const auto load_mine = load->E(-DBL_MAX);
  const auto calc_mine =
      Jessica::Calc::Geotechnical::Create::MeyerhofShallowFoundation(
          load_mine, foundation);
  JTEST_EQ(calc_mine->B_(), std::numeric_limits<double>::infinity());
}

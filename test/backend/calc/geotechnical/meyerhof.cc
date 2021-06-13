#include <cfloat>
#include <cmath>
#include <limits>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  const auto load = std::make_shared<VerticalEccentric>()
                        ->f<F::Set, F::V>(100000.)
                        ->f<F::Set, F::E>(0.25);
  const auto foundation =
      std::make_shared<FoundationStrip>()->f<F::Set, F::B>(1.);
  const auto calc = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load, foundation);

  JTEST_EQ((calc->f<F::Get, F::B_>()), 0.5);
  JTEST_EQ((calc->f<F::Get, F::Qref>()), 200000.);
  const auto load2 = load->f<F::Set, F::E>(0.5);
  const auto calc2 = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load2, foundation);
  JTEST_EQ((calc2->f<F::Get, F::B_>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<F::Get, F::Qref>()));
  const auto calc3 = calc2->f<F::Set, F::Clone>();
  JTEST_EQ((calc2->f<F::Get, F::B_>()), (calc3->f<F::Get, F::B_>()));
  JTEST_EQ((calc2->f<F::Get, F::Qref>()), (calc3->f<F::Get, F::Qref>()));

  const auto load_maxe = load->f<F::Set, F::E>(DBL_MAX);
  const auto calc_maxe = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load_maxe, foundation);
  JTEST_EQ((calc_maxe->f<F::Get, F::B_>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<F::Set, F::E>(-DBL_MAX);
  const auto calc_mine = std::make_shared<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      load_mine, foundation);
  JTEST_EQ((calc_mine->f<F::Get, F::B_>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofAllDecorator)  // NOLINT
{
  using DecoratorFoundation =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<FoundationStrip>>>>;
  using DecoratorLoad =
      DecoratorStart<LogCall<LogDuration<DecoratorEnd<VerticalEccentric>>>>;
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<DecoratorLoad, DecoratorFoundation>>>>>;

  const auto foundation = std::make_shared<DecoratorFoundation>();
  const auto foundation2 = foundation->f<F::Set, F::B>(1.);
  const auto load_1 = std::make_shared<DecoratorLoad>();
  const auto load_2 = load_1->f<F::Set, F::V>(100000.);
  const auto load = load_2->f<F::Set, F::E>(0.25);
  const auto calc = std::make_shared<DecoratorCalc>(load, foundation2);
  JTEST_EQ((calc->f<F::Get, F::B_>()), 0.5);
  JTEST_EQ((calc->f<F::Get, F::Qref>()), 200000.);
  const auto load2 = load->f<F::Set, F::E>(0.5);
  const auto calc2 = std::make_shared<DecoratorCalc>(load2, foundation2);
  JTEST_EQ((calc2->f<F::Get, F::B_>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<F::Get, F::Qref>()));
  const auto calc3 = calc2->f<F::Set, F::Clone>();
  JTEST_EQ((calc2->f<F::Get, F::B_>()), (calc3->f<F::Get, F::B_>()));
  JTEST_EQ((calc2->f<F::Get, F::Qref>()), (calc3->f<F::Get, F::Qref>()));

  const auto load_maxe = load->f<F::Set, F::E>(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(load_maxe, foundation2);
  JTEST_EQ((calc_maxe->f<F::Get, F::B_>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<F::Set, F::E>(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(load_mine, foundation2);
  JTEST_EQ((calc_mine->f<F::Get, F::B_>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofMainDecorator)  // NOLINT
{
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  const auto foundation = std::make_shared<FoundationStrip>();
  const auto foundation2 = foundation->f<F::Set, F::B>(1.);
  const auto load_1 = std::make_shared<VerticalEccentric>();
  const auto load_2 = load_1->f<F::Set, F::V>(100000.);
  const auto load = load_2->f<F::Set, F::E>(0.25);
  const auto calc = std::make_shared<DecoratorCalc>(load, foundation2);
  JTEST_EQ((calc->f<F::Get, F::B_>()), 0.5);
  JTEST_EQ((calc->f<F::Get, F::Qref>()), 200000.);
  const auto load2 = load->f<F::Set, F::E>(0.5);
  const auto calc2 = std::make_shared<DecoratorCalc>(load2, foundation2);
  JTEST_EQ((calc2->f<F::Get, F::B_>()), 0.);
  JTEST_TRUE(std::isinf(calc2->f<F::Get, F::Qref>()));
  const auto calc3 = calc2->f<F::Set, F::Clone>();
  JTEST_EQ((calc2->f<F::Get, F::B_>()), (calc3->f<F::Get, F::B_>()));
  JTEST_EQ((calc2->f<F::Get, F::Qref>()), (calc3->f<F::Get, F::Qref>()));

  const auto load_maxe = load->f<F::Set, F::E>(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(load_maxe, foundation2);
  JTEST_EQ((calc_maxe->f<F::Get, F::B_>()),
           -std::numeric_limits<double>::infinity());

  const auto load_mine = load->f<F::Set, F::E>(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(load_mine, foundation2);
  JTEST_EQ((calc_mine->f<F::Get, F::B_>()),
           std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofOnly)  // NOLINT
{
  using DecoratorCalc = DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  const auto calc =
      std::make_shared<DecoratorCalc>(std::make_shared<VerticalEccentric>(),
                                      std::make_shared<FoundationStrip>());
  const auto calc2 = calc->f<F::Set, F::Load, F::E>(0.25)
                         ->f<F::Set, F::Load, F::V>(100000.)
                         ->f<F::Set, F::Foundation, F::B>(1.);
  JTEST_EQ((calc2->f<F::Get, F::Load, F::E>()), 0.25);
  JTEST_EQ((calc2->f<F::Get, F::Load, F::V>()), 100000.);
  JTEST_EQ((calc2->f<F::Get, F::Foundation, F::B>()), 1.);
  JTEST_EQ((calc2->f<F::Get, F::B_>()), 0.5);
  JTEST_EQ((calc2->f<F::Get, F::Qref>()), 200000.);
}

#include <cfloat>
#include <cmath>
#include <limits>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  const auto load =
      std::make_shared<Jessica::Data::Load::VerticalEccentric<
          Jessica::Data::Load::VerticalEccentricImpl>>()
          ->f<true, Jessica::Data::Load::VerticalEccentricImpl::V>(100000.)
          ->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.25);
  const auto foundation =
      std::make_shared<Jessica::Data::Geotechnical::FoundationStrip<
          Jessica::Data::Geotechnical::FoundationStripImpl>>()
          ->f<true, Jessica::Data::Geotechnical::FoundationStripImpl::B>(1.);
  const auto calc =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              Jessica::Data::Load::VerticalEccentricImpl,
              Jessica::Data::Geotechnical::FoundationStripImpl>>>(load,
                                                                  foundation);

  JTEST_EQ(
      (calc->f<false,
               Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                   Jessica::Data::Load::VerticalEccentricImpl,
                   Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()),
      0.5);
  JTEST_EQ(
      (calc->f<false,
               Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                   Jessica::Data::Load::VerticalEccentricImpl,
                   Jessica::Data::Geotechnical::FoundationStripImpl>::Qref>()),
      200000.);
  const auto load2 =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.5);
  const auto calc2 =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              Jessica::Data::Load::VerticalEccentricImpl,
              Jessica::Data::Geotechnical::FoundationStripImpl>>>(load2,
                                                                  foundation);
  JTEST_EQ(
      (calc2->f<false,
                Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                    Jessica::Data::Load::VerticalEccentricImpl,
                    Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()),
      0.);
  JTEST_TRUE(std::isinf(
      calc2->f<false,
               Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                   Jessica::Data::Load::VerticalEccentricImpl,
                   Jessica::Data::Geotechnical::FoundationStripImpl>::Qref>()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ(
      (calc2->f<false,
                Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                    Jessica::Data::Load::VerticalEccentricImpl,
                    Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()),
      (calc3->f<false,
                Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                    Jessica::Data::Load::VerticalEccentricImpl,
                    Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()));
  JTEST_EQ(
      (calc2->f<false,
                Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                    Jessica::Data::Load::VerticalEccentricImpl,
                    Jessica::Data::Geotechnical::FoundationStripImpl>::Qref>()),
      (calc3
           ->f<false,
               Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                   Jessica::Data::Load::VerticalEccentricImpl,
                   Jessica::Data::Geotechnical::FoundationStripImpl>::Qref>()));

  const auto load_maxe =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              Jessica::Data::Load::VerticalEccentricImpl,
              Jessica::Data::Geotechnical::FoundationStripImpl>>>(load_maxe,
                                                                  foundation);
  JTEST_EQ(
      (calc_maxe->f<
          false, Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                     Jessica::Data::Load::VerticalEccentricImpl,
                     Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()),
      -std::numeric_limits<double>::infinity());

  const auto load_mine =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              Jessica::Data::Load::VerticalEccentricImpl,
              Jessica::Data::Geotechnical::FoundationStripImpl>>>(load_mine,
                                                                  foundation);
  JTEST_EQ(
      (calc_mine->f<
          false, Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                     Jessica::Data::Load::VerticalEccentricImpl,
                     Jessica::Data::Geotechnical::FoundationStripImpl>::B_>()),
      std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofAllDecorator)  // NOLINT
{
  using DecoratorFoundation =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Data::Geotechnical::FoundationStripImpl>>;
  using DecoratorLoad =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Data::Load::VerticalEccentricImpl>>;
  using DecoratorCalc =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              DecoratorLoad, DecoratorFoundation>>>;

  const auto foundation = std::make_shared<
      Jessica::Data::Geotechnical::FoundationStrip<DecoratorFoundation>>();
  const auto foundation2 =
      foundation->f<true, Jessica::Data::Geotechnical::FoundationStripImpl::B>(
          1.);
  const auto load_1 =
      std::make_shared<Jessica::Data::Load::VerticalEccentric<DecoratorLoad>>();
  const auto load_2 =
      load_1->f<true, Jessica::Data::Load::VerticalEccentricImpl::V>(100000.);
  const auto load =
      load_2->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.25);
  const auto calc = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load, foundation2);
  JTEST_EQ((calc->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
           0.5);
  JTEST_EQ((calc->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::Qref>()),
           200000.);
  const auto load2 =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.5);
  const auto calc2 = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load2, foundation2);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()),
           0.);
  JTEST_TRUE(std::isinf(
      calc2
          ->f<false, Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()),
           (calc3->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()));
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()),
           (calc3->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()));

  const auto load_maxe =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(DBL_MAX);
  const auto calc_maxe = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load_maxe, foundation2);
  JTEST_EQ(
      (calc_maxe->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
      -std::numeric_limits<double>::infinity());

  const auto load_mine =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(-DBL_MAX);
  const auto calc_mine = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load_mine, foundation2);
  JTEST_EQ(
      (calc_mine->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
      std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofMainDecorator)  // NOLINT
{
  using DecoratorFoundation = Jessica::Data::Geotechnical::FoundationStripImpl;
  using DecoratorLoad = Jessica::Data::Load::VerticalEccentricImpl;
  using DecoratorCalc =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              DecoratorLoad, DecoratorFoundation>>>;

  const auto foundation = std::make_shared<
      Jessica::Data::Geotechnical::FoundationStrip<DecoratorFoundation>>();
  const auto foundation2 =
      foundation->f<true, Jessica::Data::Geotechnical::FoundationStripImpl::B>(
          1.);
  const auto load_1 =
      std::make_shared<Jessica::Data::Load::VerticalEccentric<DecoratorLoad>>();
  const auto load_2 =
      load_1->f<true, Jessica::Data::Load::VerticalEccentricImpl::V>(100000.);
  const auto load =
      load_2->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.25);
  const auto calc = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load, foundation2);
  JTEST_EQ((calc->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
           0.5);
  JTEST_EQ((calc->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::Qref>()),
           200000.);
  const auto load2 =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(0.5);
  const auto calc2 = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load2, foundation2);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()),
           0.);
  JTEST_TRUE(std::isinf(
      calc2
          ->f<false, Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()),
           (calc3->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()));
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()),
           (calc3->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()));

  const auto load_maxe =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(DBL_MAX);
  const auto calc_maxe = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load_maxe, foundation2);
  JTEST_EQ(
      (calc_maxe->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
      -std::numeric_limits<double>::infinity());

  const auto load_mine =
      load->f<true, Jessica::Data::Load::VerticalEccentricImpl::E>(-DBL_MAX);
  const auto calc_mine = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>(
      load_mine, foundation2);
  JTEST_EQ(
      (calc_mine->f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        DecoratorLoad, DecoratorFoundation>::B_>()),
      std::numeric_limits<double>::infinity());
}

JTEST_NAME(data, CalcMeyehofOnly)  // NOLINT
{
  using DecoratorFoundation = Jessica::Data::Geotechnical::FoundationStripImpl;
  using DecoratorLoad = Jessica::Data::Load::VerticalEccentricImpl;
  using DecoratorCalc =
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              DecoratorLoad, DecoratorFoundation>>>;

  const auto calc = std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<DecoratorCalc>>();
  const auto calc2 =
      calc->f<true,
              Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                  DecoratorLoad, DecoratorFoundation>::Load,
              Jessica::Data::Load::VerticalEccentricImpl::E>(0.25)
          ->f<true,
              Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                  DecoratorLoad, DecoratorFoundation>::Load,
              Jessica::Data::Load::VerticalEccentricImpl::V>(100000.)
          ->f<true,
              Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                  DecoratorLoad, DecoratorFoundation>::Foundation,
              Jessica::Data::Geotechnical::FoundationStripImpl::B>(1.);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Load,
                     Jessica::Data::Load::VerticalEccentricImpl::E>()),
           0.25);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Load,
                     Jessica::Data::Load::VerticalEccentricImpl::V>()),
           100000.);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Foundation,
                     Jessica::Data::Geotechnical::FoundationStripImpl::B>()),
           1.);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::B_>()),
           0.5);
  JTEST_EQ((calc2->f<false,
                     Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                         DecoratorLoad, DecoratorFoundation>::Qref>()),
           200000.);
}

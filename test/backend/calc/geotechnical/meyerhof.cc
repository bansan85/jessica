#include <cfloat>
#include <cmath>
#include <fstream>
#include <limits>
#include <memory>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/calc/geotechnical/decorator_meyerhof.h>
#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/decorator_foundation_strip.h>
#include <jessica/data/load/decorator_vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/cereal/json.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

JTEST_NAME(data, CalcMeyehof)  // NOLINT
{
  using MSF = MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>;

  const auto load =
      std::make_shared<VerticalEccentric>()->SetV(100000.)->SetE(0.25);
  const auto foundation = std::make_shared<FoundationStrip>()->SetB(1.);
  const auto calc = std::make_shared<MSF>(load, foundation);

  JTEST_EQ(calc->B_(), 0.5);
  JTEST_EQ(calc->Qref(), 200000.);
  const auto load2 = load->SetE(0.5);
  const auto calc2 = std::make_shared<MSF>(load2, foundation);
  JTEST_EQ(calc2->B_(), 0.);
  JTEST_TRUE(std::isinf(calc2->Qref()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ(calc2->B_(), calc3->B_());
  JTEST_EQ(calc2->Qref(), calc3->Qref());

  const auto load_maxe = load->SetE(DBL_MAX);
  const auto calc_maxe = std::make_shared<MSF>(load_maxe, foundation);
  JTEST_EQ(calc_maxe->B_(), -std::numeric_limits<double>::infinity());

  const auto load_mine = load->SetE(-DBL_MAX);
  const auto calc_mine = std::make_shared<MSF>(load_mine, foundation);
  JTEST_EQ(calc_mine->B_(), std::numeric_limits<double>::infinity());

  {
    std::ofstream os("MeyerhofShallowFoundation1.cereal.json",
                     std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*calc3);
  }
  {
    std::ifstream os("MeyerhofShallowFoundation1.cereal.json",
                     std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    MSF calc4;
    iarchive(calc4);
    JTEST_EQ(calc4.B_(), calc3->B_());
    JTEST_EQ(calc4.Qref(), calc3->Qref());
  }
}

JTEST_NAME(data, CalcMeyehofAllDecorator)  // NOLINT
{
  using DecoratorFoundation = DecoratorStartFoundationStrip<
      LogCall<LogDuration<DecoratorEndFoundationStrip<FoundationStrip>>>>;
  using DecoratorLoad = DecoratorStartVerticalEccentric<
      LogCall<LogDuration<DecoratorEndVerticalEccentric<VerticalEccentric>>>>;
  using DecoratorCalc =
      DecoratorStartMeyerhof<LogCall<LogDuration<DecoratorEndMeyerhof<
          MeyerhofShallowFoundation<DecoratorLoad, DecoratorFoundation>>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<DecoratorFoundation>(log, log);
  const auto foundation2 = foundation->SetB(1.);
  const auto load_1 = std::make_shared<DecoratorLoad>(log, log);
  const auto load_2 = load_1->SetV(100000.);
  const auto load = load_2->SetE(0.25);
  const auto calc =
      std::make_shared<DecoratorCalc>(log, log, load, foundation2);
  JTEST_EQ(calc->B_(), 0.5);
  JTEST_EQ(calc->Qref(), 200000.);
  const auto load2 = load->SetE(0.5);
  const auto calc2 =
      std::make_shared<DecoratorCalc>(log, log, load2, foundation2);
  JTEST_EQ(calc2->B_(), 0.);
  JTEST_TRUE(std::isinf(calc2->Qref()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ(calc2->B_(), calc3->B_());
  JTEST_EQ(calc2->Qref(), calc3->Qref());

  const auto load_maxe = load->SetE(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(log, log, load_maxe, foundation2);
  JTEST_EQ(calc_maxe->B_(), -std::numeric_limits<double>::infinity());

  const auto load_mine = load->SetE(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(log, log, load_mine, foundation2);
  JTEST_EQ(calc_mine->B_(), std::numeric_limits<double>::infinity());

  {
    std::ofstream os("MeyerhofShallowFoundation2.cereal.json",
                     std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*calc3);
  }

  // It's not possible to deserialize nested decorator with non default
  // constructor.
}

JTEST_NAME(data, CalcMeyehofMainDecorator)  // NOLINT
{
  using DecoratorCalc =
      DecoratorStartMeyerhof<LogCall<LogDuration<DecoratorEndMeyerhof<
          MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<FoundationStrip>();
  const auto foundation2 = foundation->SetB(1.);
  const auto load_1 = std::make_shared<VerticalEccentric>();
  const auto load_2 = load_1->SetV(100000.);
  const auto load = load_2->SetE(0.25);
  const auto calc =
      std::make_shared<DecoratorCalc>(log, log, load, foundation2);
  JTEST_EQ(calc->B_(), 0.5);
  JTEST_EQ(calc->Qref(), 200000.);
  const auto load2 = load->SetE(0.5);
  const auto calc2 =
      std::make_shared<DecoratorCalc>(log, log, load2, foundation2);
  JTEST_EQ(calc2->B_(), 0.);
  JTEST_TRUE(std::isinf(calc2->Qref()));
  const auto calc3 = calc2->Clone();
  JTEST_EQ(calc2->B_(), calc3->B_());
  JTEST_EQ(calc2->Qref(), calc3->Qref());

  const auto load_maxe = load->SetE(DBL_MAX);
  const auto calc_maxe =
      std::make_shared<DecoratorCalc>(log, log, load_maxe, foundation2);
  JTEST_EQ(calc_maxe->B_(), -std::numeric_limits<double>::infinity());

  const auto load_mine = load->SetE(-DBL_MAX);
  const auto calc_mine =
      std::make_shared<DecoratorCalc>(log, log, load_mine, foundation2);
  JTEST_EQ(calc_mine->B_(), std::numeric_limits<double>::infinity());

  {
    std::ofstream os("MeyerhofShallowFoundation3.cereal.json",
                     std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*calc3);
  }
  {
    std::ifstream os("MeyerhofShallowFoundation3.cereal.json",
                     std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    DecoratorCalc calc4(log, log);
    iarchive(calc4);
    JTEST_EQ(calc4.B_(), calc3->B_());
    JTEST_EQ(calc4.Qref(), calc3->Qref());
  }
}

JTEST_NAME(data, CalcMeyehofChildrenAccess)  // NOLINT
{
  using DecoratorCalc =
      DecoratorStartMeyerhof<LogCall<LogDuration<DecoratorEndMeyerhof<
          MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>;

  auto log = spdlog::get("log");

  const auto calc = std::make_shared<DecoratorCalc>(
      log, log, std::make_shared<VerticalEccentric>(),
      std::make_shared<FoundationStrip>());
  const auto calc2 = calc->SetLoad(&VerticalEccentric::SetE, 0.25)
                         ->SetLoad(&VerticalEccentric::SetV, 100000.)
                         ->SetFoundation(&FoundationStrip::SetB, 1.);
  JTEST_EQ(calc2->Load(&VerticalEccentric::E), 0.25);
  JTEST_EQ(calc2->Load(&VerticalEccentric::V), 100000.);
  JTEST_EQ(calc2->Foundation(&FoundationStrip::B), 1.);
  JTEST_EQ(calc2->B_(), 0.5);
  JTEST_EQ(calc2->Qref(), 200000.);

  {
    std::ofstream os("MeyerhofShallowFoundation4.cereal.json",
                     std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*calc2);
  }
  {
    std::ifstream os("MeyerhofShallowFoundation4.cereal.json",
                     std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    DecoratorCalc calc3(log, log);
    iarchive(calc3);
    JTEST_EQ(calc3.B_(), calc2->B_());
    JTEST_EQ(calc3.Qref(), calc2->Qref());
  }
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

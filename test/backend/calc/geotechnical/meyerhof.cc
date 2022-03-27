// IWYU pragma: no_include <map>
// IWYU pragma: no_include "jessica/util/math/hash.h"

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <jessica/calc/geotechnical/decorator_meyerhof.h>
#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/decorator_foundation_strip.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/decorator_vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/adapter/json_parser/json_nlohmann.h>
#include <jessica/helper/adapter/json_parser/json_simdjson.h>
#include <jessica/helper/adapter/webservice/restbed.h>
#include <jessica/helper/cfi.h>
#include <jessica/helper/clean/cereal/cereal.h>
#include <jessica/helper/clean/cereal/json.h>
#include <jessica/helper/clean/spdlog/spdlog.h>
#include <jessica/helper/clean/spdlog/stdout_sinks.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

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

  {
    JsonNlohmann parser;
    parser.ReadFile("MeyerhofShallowFoundation1.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.5);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "load", "e"}, "0.3"_json)
            .Set(std::vector<std::string>{"value0", "load", "v"}, "100050"_json)
            .Set(std::vector<std::string>{"value0", "foundation", "b"},
                 "1.05"_json);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "e"})
                        .c_str(),
                    nullptr),
             0.3);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "v"})
                        .c_str(),
                    nullptr),
             100050.);
    JTEST_EQ(
        strtod(
            parser2.Get(std::vector<std::string>{"value0", "foundation", "b"})
                .c_str(),
            nullptr),
        1.05);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("MeyerhofShallowFoundation1.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.5);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
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

  {
    JsonNlohmann parser;
    parser.ReadFile("MeyerhofShallowFoundation3.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.5);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "load", "e"}, "0.3"_json)
            .Set(std::vector<std::string>{"value0", "load", "v"}, "100050"_json)
            .Set(std::vector<std::string>{"value0", "foundation", "b"},
                 "1.05"_json);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "e"})
                        .c_str(),
                    nullptr),
             0.3);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "v"})
                        .c_str(),
                    nullptr),
             100050.);
    JTEST_EQ(
        strtod(
            parser2.Get(std::vector<std::string>{"value0", "foundation", "b"})
                .c_str(),
            nullptr),
        1.05);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("MeyerhofShallowFoundation3.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.5);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
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

  {
    JsonNlohmann parser;
    parser.ReadFile("MeyerhofShallowFoundation4.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.25);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "load", "e"}, "0.3"_json)
            .Set(std::vector<std::string>{"value0", "load", "v"}, "100050"_json)
            .Set(std::vector<std::string>{"value0", "foundation", "b"},
                 "1.05"_json);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "e"})
                        .c_str(),
                    nullptr),
             0.3);
    JTEST_EQ(strtod(parser2.Get(std::vector<std::string>{"value0", "load", "v"})
                        .c_str(),
                    nullptr),
             100050.);
    JTEST_EQ(
        strtod(
            parser2.Get(std::vector<std::string>{"value0", "foundation", "b"})
                .c_str(),
            nullptr),
        1.05);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("MeyerhofShallowFoundation4.cereal.json");
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "e"}).c_str(),
            nullptr),
        0.25);
    JTEST_EQ(
        strtod(
            parser.Get(std::vector<std::string>{"value0", "load", "v"}).c_str(),
            nullptr),
        100000.);
    JTEST_EQ(
        strtod(parser.Get(std::vector<std::string>{"value0", "foundation", "b"})
                   .c_str(),
               nullptr),
        1.);
  }
}

JTEST_NAME(data, MeyerhofShallowFoundationWebServicesRestBed)  // NOLINT
{
  RestbedWs webservice;

  using MSF = MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>;

  const auto load =
      std::make_shared<VerticalEccentric>()->SetV(100000.)->SetE(0.25);
  const auto foundation = std::make_shared<FoundationStrip>()->SetB(1.);
  const auto calc = std::make_shared<MSF>(load, foundation);

  webservice.AddPath("POST", "/resource",
                     [](const std::string& data) -> std::pair<int, std::string>
                     {
                       std::stringstream is(data);
                       cereal::JSONInputArchive iarchive(is);

                       MSF sload;
                       iarchive(sload);
                       std::string retval = cfi_to_string(sload.B_()) + "," +
                                            cfi_to_string(sload.Qref());

                       return std::make_pair(200, retval);
                     });

  webservice.AddPath(
      "GET", "/quit",
      [&webservice](const std::string& /*data*/) -> std::pair<int, std::string>
      {
        webservice.Stop();
        return std::make_pair(200, "");
      });

  std::thread server([&webservice]() { webservice.Start(1984); });

  webservice.WaitStarted();

  std::stringstream stream;
  {
    cereal::JSONOutputArchive archive(stream);
    archive(*calc);
  }

  const auto [status_code, json_str] = RestbedRequest::Sync(
      "localhost", 1984, "POST", "/resource", stream.str());

  JTEST_EQ(status_code, 200);
  JTEST_EQ(json_str, "0.5,200000");

  const auto [status_code2, json_str2] =
      RestbedRequest::Sync("localhost", 1984, "GET", "/quit", "");

  JTEST_EQ(status_code2, 500);

  server.join();
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

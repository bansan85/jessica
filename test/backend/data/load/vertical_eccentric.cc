// IWYU pragma: no_include "jessica/util/math/hash.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <cereal/cereal.hpp>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/data/load/decorator_vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/adapter/json_parser/json_nlohmann.h>
#include <jessica/helper/adapter/json_parser/json_simdjson.h>
#include <jessica/helper/adapter/webservice/restbed.h>
#include <jessica/helper/cereal/json.h>
#include <jessica/helper/cfi.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

using namespace jessica;

JTEST_NAME(data, VerticalEccentric)  // NOLINT
{
  const auto load = std::make_shared<VerticalEccentric>();
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_TRUE(std::isnan(load->V()));
  const auto load2 = load->SetV(100000.);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_EQ(load2->V(), 100000.);
  const auto load3 = load2->SetE(0.2);
  JTEST_EQ(load3->E(), 0.2);
  JTEST_EQ(load3->V(), 100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ(load3->E(), load4->E());
  JTEST_EQ(load3->V(), load4->V());

  {
    std::ofstream os("VerticalEccentric1.cereal.json", std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*load4);
  }
  {
    std::ifstream os("VerticalEccentric1.cereal.json", std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    VerticalEccentric load5;
    iarchive(load5);
    JTEST_EQ(load5.E(), load4->E());
    JTEST_EQ(load5.V(), load4->V());
  }

  {
    JsonNlohmann parser;
    parser.ReadFile("VerticalEccentric1.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "e"}).c_str(),
                    nullptr),
             0.2);
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "v"}).c_str(),
                    nullptr),
             100000.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "e"}, "0.3"_json)
            .Set(std::vector<std::string>{"value0", "v"}, "100050"_json);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "e"}).c_str(),
               nullptr),
        0.3);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "v"}).c_str(),
               nullptr),
        100050.);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("VerticalEccentric1.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "e"}).c_str(),
                    nullptr),
             0.2);
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "v"}).c_str(),
                    nullptr),
             100000.);
  }
}

JTEST_NAME(data, VerticalEccentricDecorator)  // NOLINT
{
  using Decorator = DecoratorStartVerticalEccentric<
      LogCall<LogDuration<DecoratorEndVerticalEccentric<VerticalEccentric>>>>;

  auto log = spdlog::get("log");

  const auto load = std::make_shared<Decorator>(log, log);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_TRUE(std::isnan(load->V()));
  const auto load2 = load->SetV(100000.);
  JTEST_TRUE(std::isnan(load->E()));
  JTEST_EQ(load2->V(), 100000.);
  const auto load3 = load2->SetE(0.2);
  JTEST_EQ(load3->E(), 0.2);
  JTEST_EQ(load3->V(), 100000.);
  const auto load4 = load3->Clone();
  JTEST_EQ(load3->E(), load4->E());
  JTEST_EQ(load3->V(), load4->V());

  {
    std::ofstream os("VerticalEccentric2.cereal.json", std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*load4);
  }
  {
    std::ifstream os("VerticalEccentric2.cereal.json", std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    Decorator load5(log, log);
    iarchive(load5);
    JTEST_EQ(load5.E(), load4->E());
    JTEST_EQ(load5.V(), load4->V());
  }

  {
    JsonNlohmann parser;
    parser.ReadFile("VerticalEccentric2.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "e"}).c_str(),
                    nullptr),
             0.2);
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "v"}).c_str(),
                    nullptr),
             100000.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "e"}, "0.3"_json)
            .Set(std::vector<std::string>{"value0", "v"}, "100050"_json);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "e"}).c_str(),
               nullptr),
        0.3);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "v"}).c_str(),
               nullptr),
        100050.);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("VerticalEccentric2.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "e"}).c_str(),
                    nullptr),
             0.2);
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "v"}).c_str(),
                    nullptr),
             100000.);
  }
}

JTEST_NAME(data, VerticalEccentricWebServicesRestBed)  // NOLINT
{
  RestbedWs webservice;

  const auto load =
      std::make_shared<VerticalEccentric>()->SetV(100000.)->SetE(0.2);

  webservice.AddPath("POST", "/resource",
                     [](const std::string& data) -> std::pair<int, std::string>
                     {
                       std::stringstream is(data);
                       cereal::JSONInputArchive iarchive(is);

                       VerticalEccentric eccentric;
                       iarchive(eccentric);
                       std::string retval = cfi_to_string(eccentric.V()) + "," +
                                            cfi_to_string(eccentric.E());

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
    archive(*load);
  }

  const auto [status_code, json_str] = RestbedRequest::Sync(
      "localhost", 1984, "POST", "/resource", stream.str());

  JTEST_EQ(status_code, 200);
  JTEST_EQ(json_str, "100000,0.2");

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

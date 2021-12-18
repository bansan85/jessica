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
#include <vector>

#include <cereal/cereal.hpp>
#include <restbed>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/calc/geotechnical/decorator_meyerhof.h>
#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/decorator_foundation_strip.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/decorator_vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/adapter/json_parser/json_nlohmann.h>
#include <jessica/helper/adapter/json_parser/json_simdjson.h>
#include <jessica/helper/cereal/json.h>
#include <jessica/helper/cfi.h>
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
  restbed::Service service;

  using MSF = MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>;

  const auto load =
      std::make_shared<VerticalEccentric>()->SetV(100000.)->SetE(0.25);
  const auto foundation = std::make_shared<FoundationStrip>()->SetB(1.);
  const auto calc = std::make_shared<MSF>(load, foundation);

  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/resource");
  resource->set_method_handler(
      "POST",
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [](const std::shared_ptr<restbed::Session> session)
      {
        const auto request = session->get_request();

        int content_length = request->get_header("Content-Length", 0);

        session->fetch(
            static_cast<size_t>(content_length),
            // NOLINTNEXTLINE(performance-unnecessary-value-param)
            [](const std::shared_ptr<restbed::Session> sessionb,
               const restbed::Bytes& body)
            {
              // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
              std::string json_str(reinterpret_cast<const char*>(body.data()),
                                   body.size());
              std::stringstream is(json_str);
              cereal::JSONInputArchive iarchive(is);

              MSF sload;
              iarchive(sload);
              std::string retval =
                  cfi_to_string(sload.B_()) + "," + cfi_to_string(sload.Qref());

              sessionb->close(
                  restbed::OK, retval,
                  {{"Content-Length", cfi_to_string(retval.length())}});
            });
      });

  auto resource_close = std::make_shared<restbed::Resource>();
  resource_close->set_path("/quit");
  resource_close->set_method_handler(
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      "GET", [&service](const std::shared_ptr<restbed::Session> /*session*/)
      { service.stop(); });

  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(1984);
  settings->set_default_header("Connection", "close");

  service.publish(resource);
  service.publish(resource_close);

  std::thread server([&service, &settings]() { service.start(settings); });

  while (!service.is_up())
  {
  }
  {
    std::stringstream stream;
    {
      cereal::JSONOutputArchive archive(stream);
      archive(*calc);
    }

    auto request = std::make_shared<restbed::Request>();
    request->set_method("POST");
    request->set_port(1984);
    request->set_host("localhost");
    request->set_path("/resource");

    request->set_body(stream.str());

    request->set_header("Content-Length", cfi_to_string(stream.str().size()));

    auto response = restbed::Http::sync(request);
    restbed::Http::fetch(std::stoul(response->get_header("Content-Length")),
                         response);

    JTEST_EQ(response->get_status_code(), 200);
    std::string json_str(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const char*>(response->get_body().data()),
        response->get_body().size());
    JTEST_EQ(json_str, "0.5,200000");
  }
  {
    auto request = std::make_shared<restbed::Request>();
    request->set_method("GET");
    request->set_port(1984);
    request->set_host("localhost");
    request->set_path("/quit");

    auto response = restbed::Http::sync(request);
  }

  server.join();
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

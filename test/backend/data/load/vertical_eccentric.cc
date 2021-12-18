// IWYU pragma: no_include "jessica/util/math/hash.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <cereal/cereal.hpp>
#include <restbed>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

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
  restbed::Service service;

  const auto load =
      std::make_shared<VerticalEccentric>()->SetV(100000.)->SetE(0.2);

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

              VerticalEccentric eccentric;
              iarchive(eccentric);
              std::string retval = cfi_to_string(eccentric.V()) + "," +
                                   cfi_to_string(eccentric.E());

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
      archive(*load);
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
    JTEST_EQ(json_str, "100000,0.2");
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

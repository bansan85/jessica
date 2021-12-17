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

#include <jessica/data/geotechnical/decorator_foundation_strip.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/helper/adapter/json_parser/json_nlohmann.h>
#include <jessica/helper/adapter/json_parser/json_simdjson.h>
#include <jessica/helper/cereal/json.h>
#include <jessica/helper/cfi.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

using namespace jessica;

JTEST_NAME(data, FoundationStrip)  // NOLINT
{
  const auto foundation = std::make_shared<FoundationStrip>();
  JTEST_TRUE(std::isnan(foundation->B()));
  const auto foundation2 = foundation->SetB(1.);
  JTEST_TRUE(std::isnan(foundation->B()));
  JTEST_EQ(foundation2->B(), 1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ(foundation2->B(), foundation3->B());

  {
    std::ofstream os("FoundationStrip1.cereal.json", std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*foundation3);
  }
  {
    std::ifstream os("FoundationStrip1.cereal.json", std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    FoundationStrip foundation4;
    iarchive(foundation4);
    JTEST_EQ(foundation4.B(), foundation3->B());
  }

  {
    JsonNlohmann parser;
    parser.ReadFile("FoundationStrip1.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "b"}).c_str(),
                    nullptr),
             1.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "b"}, "2.0"_json);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "b"}).c_str(),
               nullptr),
        2.);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("FoundationStrip1.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "b"}).c_str(),
                    nullptr),
             1.);
  }
}

JTEST_NAME(data, FoundationStripDecorator)  // NOLINT
{
  using Decorator = DecoratorStartFoundationStrip<
      LogCall<LogDuration<DecoratorEndFoundationStrip<FoundationStrip>>>>;

  auto log = spdlog::get("log");

  const auto foundation = std::make_shared<Decorator>(log, log);
  JTEST_TRUE(std::isnan(foundation->B()));
  const auto foundation2 = foundation->SetB(1.);
  JTEST_TRUE(std::isnan(foundation->B()));
  JTEST_EQ(foundation2->B(), 1.);
  const auto foundation3 = foundation2->Clone();
  JTEST_EQ(foundation2->B(), foundation3->B());

  {
    std::ofstream os("FoundationStrip2.cereal.json", std::ios::binary);
    cereal::JSONOutputArchive oarchive(os);
    oarchive(*foundation3);
  }
  {
    std::ifstream os("FoundationStrip2.cereal.json", std::ios::binary);
    cereal::JSONInputArchive iarchive(os);

    Decorator foundation4(log, log);
    iarchive(foundation4);
    JTEST_EQ(foundation4.B(), foundation3->B());
  }

  {
    JsonNlohmann parser;
    parser.ReadFile("FoundationStrip2.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "b"}).c_str(),
                    nullptr),
             1.);
    JsonNlohmann parser2 =
        parser.Set(std::vector<std::string>{"value0", "b"}, "2.0"_json);
    JTEST_EQ(
        strtod(parser2.Get(std::vector<std::string>{"value0", "b"}).c_str(),
               nullptr),
        2.);
  }
  {
    JsonSimdjson parser;
    parser.ReadFile("FoundationStrip2.cereal.json");
    JTEST_EQ(strtod(parser.Get(std::vector<std::string>{"value0", "b"}).c_str(),
                    nullptr),
             1.);
  }
}

JTEST_NAME(data, WebServices)  // NOLINT
{
  restbed::Service service;

  const auto foundation = std::make_shared<FoundationStrip>()->SetB(1.);

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

              FoundationStrip foundation4;
              iarchive(foundation4);
              std::string retval = cfi_to_string(foundation4.B());

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
      archive(*foundation);
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
    JTEST_EQ(std::stod(json_str), 1.0);
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

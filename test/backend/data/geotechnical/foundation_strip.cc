// IWYU pragma: no_include "jessica/util/math/hash.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <cereal/cereal.hpp>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/data/geotechnical/decorator_foundation_strip.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/helper/adapter/json_parser/json_nlohmann.h>
#include <jessica/helper/adapter/json_parser/json_simdjson.h>
#include <jessica/helper/cereal/json.h>
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

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  spdlog::stdout_logger_mt("log");
  return RUN_ALL_TESTS();
}

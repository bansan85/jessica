#pragma once

#include <concepts>
#include <memory>
#include <string>

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wambiguous-reversed-operator"
  #pragma clang diagnostic ignored "-Wcovered-switch-default"
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wextra-semi"
  #pragma clang diagnostic ignored "-Wextra-semi-stmt"
  #pragma clang diagnostic ignored "-Wnewline-eof"
  #pragma clang diagnostic ignored "-Wsign-conversion"
  #pragma clang diagnostic ignored "-Wsuggest-override"
  #pragma clang diagnostic ignored "-Wswitch-enum"
  #pragma clang diagnostic ignored "-Wundef"
  #pragma clang diagnostic ignored "-Wunused-member-function"
  #pragma clang diagnostic ignored "-Wunused-template"
  #pragma clang diagnostic ignored "-Wused-but-marked-unused"
#endif
#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable : 4061)
  #pragma warning(disable : 4100)
  #pragma warning(disable : 4365)
  #pragma warning(disable : 4459)
  #pragma warning(disable : 4706)
  #pragma warning(disable : 4800)
  #pragma warning(disable : 4866)
#endif

#include <simdjson.h>

#ifdef __clang__
  #pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#include <jessica/helper/adapter/json_parser/json_parser.h>

namespace jessica
{
class JsonSimdjson
{
 public:
  JsonSimdjson() = default;
  JsonSimdjson(const JsonSimdjson&) = delete;
  JsonSimdjson(JsonSimdjson&&) = delete;
  JsonSimdjson& operator=(const JsonSimdjson&) = delete;
  JsonSimdjson& operator=(JsonSimdjson&&) = delete;

  void ReadStr(const std::string& text)
  {
    padded = simdjson::padded_string(text);
    doc = parser.iterate(padded);
  }

  void ReadFile(const std::string& file)
  {
    padded = simdjson::padded_string::load(file);
    doc = parser.iterate(padded);
  }

  std::string Get(const std::vector<std::string>& list) const
  {
    doc.rewind();
    std::string_view v;
    auto error = simdjson::to_json_string(Get(doc.get_value(), list, 0)).get(v);
    if (error == simdjson::SUCCESS)
    {
      return std::string{v};
    }
    return {};
  }

  ~JsonSimdjson() = default;

 private:
  simdjson::ondemand::parser parser;
  simdjson::padded_string padded;
  // Parsing document need to modify document iterator.
  mutable simdjson::ondemand::document doc;

  static simdjson::simdjson_result<simdjson::fallback::ondemand::value>
      // NOLINTNEXTLINE(misc-no-recursion)
      Get(simdjson::simdjson_result<simdjson::fallback::ondemand::value> data,
          const std::vector<std::string>& list, size_t i)
  {
    simdjson::simdjson_result<simdjson::fallback::ondemand::value> retval2 =
        [&data, &list, i]()
    {
      simdjson::simdjson_result<simdjson::fallback::ondemand::value> retval =
          data[list[i]];
      if (retval["decorator"].error() == simdjson::SUCCESS)
      {
        return retval["decorator"]["ptr_wrapper"]["data"];
      }
      if (retval["ptr_wrapper"].error() == simdjson::SUCCESS)
      {
        return retval["ptr_wrapper"]["data"];
      }
      return retval;
    }();

    if (i == list.size() - 1)
    {
      return retval2;
    }
    return Get(retval2, list, i + 1);
  }
};
static_assert(JsonParserReader<JsonSimdjson>);
}  // namespace jessica

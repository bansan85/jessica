#pragma once

#include <concepts>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include <jessica/helper/adapter/json_parser/json_parser.h>

namespace jessica
{
template <class Type>
struct S;
class JsonNlohmann
{
 public:
  JsonNlohmann() = default;
  JsonNlohmann(const JsonNlohmann&) = default;
  JsonNlohmann(JsonNlohmann&&) = default;
  JsonNlohmann& operator=(const JsonNlohmann&) = delete;
  JsonNlohmann& operator=(JsonNlohmann&&) = delete;

  void ReadStr(const std::string& text) { doc = nlohmann::json::parse(text); }

  void ReadFile(const std::string& file)
  {
    std::ifstream ifs(file);
    doc = nlohmann::json::parse(ifs);
  }

  [[nodiscard]] std::string Get(const std::vector<std::string>& list) const
  {
    std::string x = Get(doc, list, 0).dump();
    return x;
  }

  [[nodiscard]] JsonNlohmann Set(const std::vector<std::string>& list,
                                 const nlohmann::json& val) const
  {
    JsonNlohmann retval(*this);
    Get(retval.doc, list, 0) = val;
    return retval;
  }

  ~JsonNlohmann() = default;

 private:
  nlohmann::json doc;

  // NOLINTNEXTLINE(misc-no-recursion)
  static const nlohmann::json& Get(const nlohmann::json& data,
                                   const std::vector<std::string>& list,
                                   size_t i)
  {
    const nlohmann::json* retval2 = [&data, &list, i]()
    {
      const nlohmann::json* retval = &data[list[i]];
      auto decorator = retval->find("decorator");
      if (decorator != retval->end())
      {
        return &(*retval)["decorator"]["ptr_wrapper"]["data"];
      }
      auto ptr_wrapper = retval->find("ptr_wrapper");
      if (ptr_wrapper != retval->end())
      {
        return &(*retval)["ptr_wrapper"]["data"];
      }
      return retval;
    }();

    if (i == list.size() - 1)
    {
      return *retval2;
    }
    return Get(*retval2, list, i + 1);
  }

  static nlohmann::json& Get(nlohmann::json& data,
                             const std::vector<std::string>& list, size_t i)
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<nlohmann::json&>(
        Get(static_cast<const nlohmann::json&>(data), list, i));
  }
};
//static_assert(JsonParser<JsonNlohmann, nlohmann::json>);
}  // namespace jessica

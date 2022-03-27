#pragma once

#include <map>
#include <string>
#include <utility>

#include <jessica/helper/clean/drogon/drogon.h>
#include <jessica/helper/constexpr.h>
#include <jessica/helper/poo.h>

namespace jessica
{
class DrogonWs
{
 public:
  DrogonWs();
  RULE_OF_FIVE_NO_COPY(DrogonWs)

  void AddPath(
      const std::string& method, const std::string& path,
      const std::function<std::pair<int, std::string>(const std::string&)>&
          func);
  void Start(unsigned short port);
  void WaitStarted();
  void Stop();

  static constexpr drogon::HttpMethod
      GetHttpMethodFromString(std::string_view sv)
  {
    constexpr auto map =
        ConstExprMap<std::string_view, drogon::HttpMethod, 7>(http_method_);

    return map.at(sv);
  }

 private:
  static constexpr std::array<std::pair<std::string_view, drogon::HttpMethod>,
                              7>
      http_method_{{{"GET", drogon::HttpMethod::Get},
                    {"POST", drogon::HttpMethod::Post},
                    {"HEAD", drogon::HttpMethod::Head},
                    {"PUT", drogon::HttpMethod::Put},
                    {"DELETE", drogon::HttpMethod::Delete},
                    {"OPTIONS", drogon::HttpMethod::Options},
                    {"PATCH", drogon::HttpMethod::Patch}}};
  drogon::HttpAppFramework& app_;
};

class DrogonRequest
{
 public:
  static std::pair<int, std::string> Sync(const std::string& hostname,
                                          unsigned short port,
                                          const std::string& method,
                                          const std::string& path,
                                          const std::string& body);
};
}  // namespace jessica

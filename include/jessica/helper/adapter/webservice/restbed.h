#pragma once

#include <string>
#include <utility>

#include <restbed>

#include <jessica/helper/poo.h>

namespace jessica
{
class RestbedWs
{
 public:
  RestbedWs() = default;
  RULE_OF_FIVE_NO_COPY(RestbedWs)

  void AddPath(
      const std::string& method, const std::string& path,
      const std::function<std::pair<int, std::string>(const std::string&)>&
          func);
  void Start(unsigned short port);
  void WaitStarted();
  void Stop();

 private:
  restbed::Service service_;
};

class RestbedRequest
{
 public:
  static std::pair<int, std::string> Sync(const std::string& hostname,
                                          unsigned short port,
                                          const std::string& method,
                                          const std::string& path,
                                          const std::string& body);
};
}  // namespace jessica

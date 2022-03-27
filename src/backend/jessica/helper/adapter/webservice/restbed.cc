#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include <restbed>

#include <jessica/helper/adapter/webservice/restbed.h>
#include <jessica/helper/cfi.h>

namespace jessica
{
void RestbedWs::AddPath(
    const std::string& method, const std::string& path,
    const std::function<std::pair<int, std::string>(const std::string&)>& func)
{
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path(path);
  resource->set_method_handler(
      method,
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [func](const std::shared_ptr<restbed::Session> session)
      {
        const auto request = session->get_request();

        int content_length = request->get_header("Content-Length", 0);

        session->fetch(
            static_cast<size_t>(content_length),
            // NOLINTNEXTLINE(performance-unnecessary-value-param)
            [func](const std::shared_ptr<restbed::Session> sessionb,
                   const restbed::Bytes& body)
            {
              // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
              std::string json_str(reinterpret_cast<const char*>(body.data()),
                                   body.size());

              const auto [status_code, retval] = func(json_str);

              sessionb->close(
                  status_code, retval,
                  {{"Content-Length", cfi_to_string(retval.length())}});
            });
      });

  service_.publish(resource);
}
void RestbedWs::Start(unsigned short port)
{
  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(port);
  settings->set_default_header("Connection", "close");

  service_.start(settings);
}

void RestbedWs::Stop() { service_.stop(); }

void RestbedWs::WaitStarted()
{
  while (!service_.is_up())
  {
  }
}

std::pair<int, std::string> RestbedRequest::Sync(const std::string& hostname,
                                                 unsigned short port,
                                                 const std::string& method,
                                                 const std::string& path,
                                                 const std::string& body)
{
  auto request = std::make_shared<restbed::Request>();
  request->set_method(method);
  request->set_port(port);
  request->set_host(hostname);
  request->set_path(path);

  request->set_body(body);

  request->set_header("Content-Length", cfi_to_string(body.size()));

  auto response = restbed::Http::sync(request);

  try
  {
    restbed::Http::fetch(
        cfi_to_number<size_t>(response->get_header("Content-Length")),
        response);

    std::string data(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const char*>(response->get_body().data()),
        response->get_body().size());

    return std::make_pair(response->get_status_code(), data);
  }
  catch (const std::exception& e)
  {
    return {500, e.what()};
  }
}

}  // namespace jessica

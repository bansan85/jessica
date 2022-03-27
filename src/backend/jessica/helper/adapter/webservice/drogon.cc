// IWYU pragma: no_include <cxxabi.h>

#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#include <jessica/helper/adapter/webservice/drogon.h>
#include <jessica/helper/cfi.h>
#include <jessica/helper/clean/drogon/drogon.h>

namespace jessica
{

DrogonWs::DrogonWs() : app_(drogon::app()) {}

void DrogonWs::AddPath(
    const std::string& method, const std::string& path,
    const std::function<std::pair<int, std::string>(const std::string&)>& func)
{
  app_.registerHandler(
      path,
      [func](const drogon::HttpRequestPtr& req,
             std::function<void(const drogon::HttpResponsePtr&)>&& callback)
      {
        const auto [status_code, retval] = func(std::string{req->getBody()});

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody(retval);
        resp->setStatusCode(static_cast<drogon::HttpStatusCode>(status_code));
        callback(resp);
      },
      {GetHttpMethodFromString(method)});
}

void DrogonWs::Start(unsigned short port)
{
  app_.addListener("0.0.0.0", port).run();
}

void DrogonWs::Stop() { app_.quit(); }

void DrogonWs::WaitStarted()
{
  using namespace std::chrono_literals;
  while (!app_.isRunning())
  {
  }

  // If no sleep, error 500 for the first request.
  std::this_thread::sleep_for(20ms);
}

std::pair<int, std::string> DrogonRequest::Sync(const std::string& hostname,
                                                unsigned short port,
                                                const std::string& method,
                                                const std::string& path,
                                                const std::string& body)
{
  using namespace std::chrono_literals;
  auto client =
      drogon::HttpClient::newHttpClient(hostname + ":" + cfi_to_string(port),
                                        drogon::app().getLoop(), false, false);
  auto req = drogon::HttpRequest::newHttpRequest();
  req->setMethod(DrogonWs::GetHttpMethodFromString(method));
  req->setPath(path);
  req->setBody(body);

  std::future<std::pair<int, std::string>> future = std::async(
      std::launch::async,
      [&client, &req]() -> std::pair<int, std::string>
      {
        auto [result, response] = client->sendRequest(req, 1.);
        return {response->statusCode(), std::string{response->body()}};
      });

  std::future_status status = future.wait_for(1s);
  if (status == std::future_status::timeout)
  {
    std::terminate();
  }
  else
  {
    return future.get();
  }
}

}  // namespace jessica

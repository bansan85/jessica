#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace jessica
{
class LoggerSpdlog
{
  LoggerSpdlog() = delete;
  LoggerSpdlog(const std::shared_ptr<spdlog::logger>& spdlog) : logger_(spdlog)
  {
  }
  LoggerSpdlog(const LoggerSpdlog&) = default;
  LoggerSpdlog(LoggerSpdlog&&) = delete;
  LoggerSpdlog& operator=(const LoggerSpdlog&) = delete;
  LoggerSpdlog& operator=(LoggerSpdlog&&) = delete;

 private:
  std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace jessica

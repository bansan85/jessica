#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include <jessica/helper/accessor.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogCall
{
 public:
  using RootType = typename T::RootType;

  template <typename... Args>
  LogCall(std::shared_ptr<RootType>& impl,
          const std::shared_ptr<spdlog::logger>& log, Args&&... args)
      : t(impl, std::forward<Args>(args)...), log_(log)
  {
  }
  LogCall(const LogCall&) = default;
  LogCall(LogCall&&) = delete;
  LogCall& operator=(const LogCall&) = delete;
  LogCall& operator=(LogCall&&) = delete;

  ~LogCall() = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const RootType& classe, const Args&&... args) const
  {
    log_->info("DecoratorLogger " + std::string{typeid(T).name()});
    return t.template f<Action, U...>(classe,
                                      std::forward<const Args>(args)...);
  }

 private:
  T t;

  std::shared_ptr<spdlog::logger> log_;
};
}  // namespace jessica

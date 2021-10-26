#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include <jessica/helper/poo.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogCall
{
 public:
  using RootType = typename T::RootType;

  template <typename... Args>
  LogCall(std::shared_ptr<RootType>& impl, std::shared_ptr<spdlog::logger> log,
          Args&&... args)
      : t(impl, std::forward<Args>(args)...), log_(std::move(log))
  {
  }
  RULE_OF_FIVE_COPY(LogCall)

  template <uint64_t Action, uint64_t... U, typename... Args>
  [[nodiscard]] auto f(const RootType& classe, Args&&... args) const
  {
    log_->info("DecoratorLogger " + std::string{typeid(T).name()});
    return t.template f<Action, U...>(classe, std::forward<Args>(args)...);
  }

 private:
  T t;

  std::shared_ptr<spdlog::logger> log_;
};
}  // namespace jessica

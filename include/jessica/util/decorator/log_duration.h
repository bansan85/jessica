#pragma once

#include <chrono>
#include <cstdint>
#include <memory>

#include <jessica/helper/cfi.h>
#include <jessica/helper/clean/spdlog/spdlog.h>
#include <jessica/helper/poo.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogDuration
{
 public:
  using RootType = typename T::RootType;

  template <typename... Args>
  LogDuration(std::shared_ptr<RootType>& impl,
              std::shared_ptr<spdlog::logger> log, Args&&... args)
      : t(impl, std::forward<Args>(args)...), log_(std::move(log))
  {
  }
  RULE_OF_FIVE_COPY(LogDuration)

  template <uint64_t Action, uint64_t... U, typename... Args>
  [[nodiscard]] auto f(const RootType& classe, Args&&... args) const
  {
    const auto t_start = std::chrono::high_resolution_clock::now();
    // cppcheck-suppress redundantInitialization
    auto retval =
        t.template f<Action, U...>(classe, std::forward<Args>(args)...);
    const auto t_end = std::chrono::high_resolution_clock::now();
    const double elapsed_time_ms =
        std::chrono::duration<double, std::milli>(t_end - t_start).count();
    log_->info("duration " + cfi_to_string(elapsed_time_ms) + " ms");
    return retval;
  }

 private:
  T t;

  std::shared_ptr<spdlog::logger> log_;
};
}  // namespace jessica

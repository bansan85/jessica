#pragma once

#include <chrono>
#include <iostream>
#include <memory>

#include <jessica/helper/accessor.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogDuration : public T
{
 public:
  ~LogDuration() override = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const typename T::RootType& classe,
                       const Args&&... args) const
  {
    const auto t_start = std::chrono::high_resolution_clock::now();
    const auto retval =
        T::template f<Action, U...>(classe, std::forward<const Args>(args)...);
    const auto t_end = std::chrono::high_resolution_clock::now();
    const double elapsed_time_ms =
        std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout << "duration " << elapsed_time_ms << " ms" << std::endl;
    return retval;
  }
};
}  // namespace jessica

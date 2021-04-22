#pragma once

#include <chrono>
#include <iostream>
#include <memory>

#include <jessica/helper/template.h>

namespace Jessica::Util::Decorator
{
template <typename T>
class JESSICA_DLL_PUBLIC LogDuration
{
 public:
  using Root = typename Jessica::Helper::ExtractRootType<T>::Root;

  template <typename Args>
  static auto f(const Root& classe, const Args&& args)
  {
    const auto t_start = std::chrono::high_resolution_clock::now();
    const auto retval = T::template f(classe, std::forward<const Args>(args));
    const auto t_end = std::chrono::high_resolution_clock::now();
    const double elapsed_time_ms =
        std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout << "duration " << elapsed_time_ms << " ms" << std::endl;
    return retval;
  }

  template <typename Args>
  static auto f(const Root& classe)
  {
    const auto t_start = std::chrono::high_resolution_clock::now();
    const auto retval = T::template f<Args>(classe);
    const auto t_end = std::chrono::high_resolution_clock::now();
    const double elapsed_time_ms =
        std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout << "duration " << elapsed_time_ms << " ms" << std::endl;
    return retval;
  }
};
}  // namespace Jessica::Util::Decorator

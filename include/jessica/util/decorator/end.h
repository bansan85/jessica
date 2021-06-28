#pragma once

#include <chrono>
#include <iostream>
#include <memory>

#include <jessica/helper/accessor.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC DecoratorEnd
{
 public:
  using RootType = T;

  template <typename... Args>
  explicit DecoratorEnd(std::shared_ptr<T>& impl, Args&&... args)
  {
    impl = std::make_shared<T>(std::forward<Args>(args)...);
  }
  DecoratorEnd(const DecoratorEnd&) = default;
  DecoratorEnd(DecoratorEnd&&) = delete;
  DecoratorEnd& operator=(const DecoratorEnd&) = delete;
  DecoratorEnd& operator=(DecoratorEnd&&) = delete;

  ~DecoratorEnd() = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const T& classe, const Args&&... args) const
  {
    return classe.template f<Action, U...>(std::forward<const Args>(args)...);
  }
};
}  // namespace jessica

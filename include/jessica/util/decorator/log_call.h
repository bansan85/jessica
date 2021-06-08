#pragma once

#include <iostream>
#include <memory>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <typename T, typename T0>
class JESSICA_DLL_PUBLIC LogCall final
{
 public:
  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const T0& classe, const Args&&... args)
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return classe->template f<Action, U...>(std::forward<const Args>(args)...);
  }
};
}  // namespace jessica

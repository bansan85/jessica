#pragma once

#include <iostream>
#include <memory>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogCall
{
 public:
  using Type = typename ExtractRootType<T>::type;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] static auto f(const Type& classe, const Args&&... args)
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return T::template f<Action, U...>(classe,
                                       std::forward<const Args>(args)...);
  }
};
}  // namespace jessica

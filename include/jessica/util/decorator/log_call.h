#pragma once

#include <iostream>
#include <memory>

#include <jessica/helper/template.h>

namespace Jessica::Util::Decorator
{
template <typename T>
class JESSICA_DLL_PUBLIC LogCall
{
 public:
  using Type = typename Jessica::Helper::ExtractRootType<T>::type;

  template <bool CloneT, Jessica::Helper::F... U, typename... Args>
  [[nodiscard]] static auto f(const Type& classe, const Args&&... args)
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return T::template f<CloneT, U...>(classe,
                                       std::forward<const Args>(args)...);
  }
};
}  // namespace Jessica::Util::Decorator

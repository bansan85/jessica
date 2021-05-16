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

  template <bool CloneT, typename... U, typename... Args>
  [[nodiscard]] static auto f(const Type& classe, const Args&&... args)
  {
    using U0 = typename Jessica::Helper::ExtractNthType<0, U...>::type;
    std::cout << "DecoratorLogger " << typeid(T).name() << " "
              << typeid(U0).name() << std::endl;
    return T::template f<CloneT, U...>(classe,
                                       std::forward<const Args>(args)...);
  }
};
}  // namespace Jessica::Util::Decorator

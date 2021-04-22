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
  using Root = typename Jessica::Helper::ExtractRootType<T>::Root;

  template <typename Args>
  static auto f(const Root& classe, const Args&& args)
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << " "
              << typeid(Args).name() << std::endl;
    return T::template f(classe, std::forward<const Args>(args));
  }

  template <typename Args>
  static auto f(const Root& classe)
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << " "
              << typeid(Args).name() << std::endl;
    return T::template f<Args>(classe);
  }
};
}  // namespace Jessica::Util::Decorator

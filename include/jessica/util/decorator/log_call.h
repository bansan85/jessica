#pragma once

#include <iostream>
#include <memory>

#include <jessica/helper/accessor.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC LogCall : public T
{
 public:
  ~LogCall() override = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const typename T::RootType& classe,
                       const Args&&... args) const
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return T::template f<Action, U...>(classe,
                                       std::forward<const Args>(args)...);
  }
};
}  // namespace jessica

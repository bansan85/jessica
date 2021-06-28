#pragma once

#include <memory>

#include <jessica/helper/accessor.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC DecoratorStart final
{
 public:
  template <typename... Args>
  explicit DecoratorStart(Args&&... args)
      : deco_(std::make_shared<T>(impl_, std::forward<Args>(args)...))
  {
  }
  DecoratorStart(const DecoratorStart&) = default;
  DecoratorStart(DecoratorStart&&) = delete;
  DecoratorStart& operator=(const DecoratorStart&) = delete;
  DecoratorStart& operator=(DecoratorStart&&) = delete;

  ~DecoratorStart() = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (Action == F::Set)
    {
      auto retval = f<F::Set, F::Clone>();
      retval->impl_ = deco_->template f<Action, U...>(
          *impl_, std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return deco_->template f<Action, U...>(*impl_,
                                             std::forward<const Args>(args)...);
    }
  }

  template <F Action, F U>
  requires Equals<F, Action, F::Set> && Equals<F, U, F::Clone>
  [[nodiscard]] std::shared_ptr<DecoratorStart<T>> f() const
  {
    return std::make_shared<DecoratorStart<T>>(*this);
  }

 private:
  std::shared_ptr<typename T::RootType> impl_;
  std::shared_ptr<T> deco_;
};
}  // namespace jessica

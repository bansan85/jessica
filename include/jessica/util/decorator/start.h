#pragma once

#include <cstdint>
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

  template <uint64_t Action, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Get"_f>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    return deco_->template f<Action, U...>(*impl_,
                                           std::forward<const Args>(args)...);
  }

  template <uint64_t Action, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Set"_f>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->impl_ = deco_->template f<Action, U...>(
        *impl_, std::forward<const Args>(args)...);
    return retval;
  }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "Clone"_f>
  [[nodiscard]] std::shared_ptr<DecoratorStart<T>> f() const
  {
    return std::make_shared<DecoratorStart<T>>(*this);
  }

 private:
  std::shared_ptr<typename T::RootType> impl_;
  std::shared_ptr<T> deco_;
};
}  // namespace jessica

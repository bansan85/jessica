#pragma once

#include <cstdint>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <typename T, typename ShrT>
class JESSICA_DLL_PUBLIC DecoratorStart
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

  virtual ~DecoratorStart() = default;

  template <uint64_t Action, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Get"_f>
  [[nodiscard]] auto f(Args&&... args) const
  {
    return deco_->template f<Action, U...>(*impl_, std::forward<Args>(args)...);
  }

  template <uint64_t Action, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Set"_f>
  [[nodiscard]] auto f(Args&&... args) const
  {
    auto retval = Clone();
    retval->impl_ =
        deco_->template f<Action, U...>(*impl_, std::forward<Args>(args)...);
    return retval;
  }

  [[nodiscard]] virtual std::shared_ptr<ShrT> Clone() const = 0;

 private:
  std::shared_ptr<typename T::RootType> impl_;
  std::shared_ptr<T> deco_;
};
}  // namespace jessica

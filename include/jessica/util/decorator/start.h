#pragma once

#include <cstdint>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/clean/cereal/memory.h>
#include <jessica/helper/poo.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <typename T, typename ShrT>
class DecoratorStart
{
 public:
  template <typename... Args>
  explicit DecoratorStart(Args&&... args)
      : deco_(std::make_shared<T>(impl_, std::forward<Args>(args)...))
  {
  }
  RULE_OF_FIVE_COPY_VIRTUAL(DecoratorStart)

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

  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(cereal::make_nvp("decorator", impl_));
  }

 private:
  std::shared_ptr<typename T::RootType> impl_;
  std::shared_ptr<T> deco_;
};
}  // namespace jessica

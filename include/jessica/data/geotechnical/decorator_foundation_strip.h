#pragma once

#include <cstddef>
#include <memory>
#include <utility>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/start.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC DecoratorStartFoundationStrip
    : public DecoratorStart<T, DecoratorStartFoundationStrip<T>>
{
 public:
  template <typename... Args>
  explicit DecoratorStartFoundationStrip(Args&&... args)
      : DecoratorStart<T, DecoratorStartFoundationStrip<T>>(
            std::forward<Args>(args)...)
  {
  }
  DecoratorStartFoundationStrip(const DecoratorStartFoundationStrip&) = default;
  DecoratorStartFoundationStrip(DecoratorStartFoundationStrip&&) = delete;
  DecoratorStartFoundationStrip&
      operator=(const DecoratorStartFoundationStrip&) = delete;
  DecoratorStartFoundationStrip&
      operator=(DecoratorStartFoundationStrip&&) = delete;

  ~DecoratorStartFoundationStrip() override = default;

  [[nodiscard]] std::shared_ptr<DecoratorStartFoundationStrip<T>>
      Clone() const override
  {
    return std::make_shared<DecoratorStartFoundationStrip<T>>(*this);
  }

  [[nodiscard]] double B() const { return this->template f<"Get"_f, "B"_f>(); }

  [[nodiscard]] auto SetB(double b) const
  {
    return std::static_pointer_cast<DecoratorStartFoundationStrip<T>>(
        this->template f<"Set"_f, "B"_f>(b));
  }
};

template <typename T>
class JESSICA_DLL_PUBLIC DecoratorEndFoundationStrip : public DecoratorEnd<T>
{
 public:
  using RootType = T;

  template <typename... Args>
  explicit DecoratorEndFoundationStrip(std::shared_ptr<T>& impl, Args&&... args)
      : DecoratorEnd<T>(impl, std::forward<Args>(args)...)
  {
  }
  DecoratorEndFoundationStrip(const DecoratorEndFoundationStrip&) = default;
  DecoratorEndFoundationStrip(DecoratorEndFoundationStrip&&) = delete;
  DecoratorEndFoundationStrip&
      operator=(const DecoratorEndFoundationStrip&) = delete;
  DecoratorEndFoundationStrip&
      operator=(DecoratorEndFoundationStrip&&) = delete;

  ~DecoratorEndFoundationStrip() override = default;

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "B"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.B(); }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "B"_f>
  [[nodiscard]] auto f(const T& classe, const double b) const
  {
    return classe.SetB(b);
  }
};
}  // namespace jessica

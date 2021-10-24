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
class JESSICA_DLL_PUBLIC DecoratorStartVerticalEccentric
    : public DecoratorStart<T, DecoratorStartVerticalEccentric<T>>
{
 public:
  template <typename... Args>
  explicit DecoratorStartVerticalEccentric(Args&&... args)
      : DecoratorStart<T, DecoratorStartVerticalEccentric<T>>(
            std::forward<Args>(args)...)
  {
  }
  DecoratorStartVerticalEccentric(const DecoratorStartVerticalEccentric&) =
      default;
  DecoratorStartVerticalEccentric(DecoratorStartVerticalEccentric&&) = delete;
  DecoratorStartVerticalEccentric&
      operator=(const DecoratorStartVerticalEccentric&) = delete;
  DecoratorStartVerticalEccentric&
      operator=(DecoratorStartVerticalEccentric&&) = delete;

  ~DecoratorStartVerticalEccentric() override = default;

  [[nodiscard]] std::shared_ptr<DecoratorStartVerticalEccentric<T>>
      Clone() const override
  {
    return std::make_shared<DecoratorStartVerticalEccentric<T>>(*this);
  }

  [[nodiscard]] double E() const { return this->template f<"Get"_f, "E"_f>(); }

  [[nodiscard]] auto SetE(double e) const
  {
    return std::static_pointer_cast<DecoratorStartVerticalEccentric<T>>(
        this->template f<"Set"_f, "E"_f>(e));
  }

  [[nodiscard]] double V() const { return this->template f<"Get"_f, "V"_f>(); }

  [[nodiscard]] auto SetV(double v) const
  {
    return std::static_pointer_cast<DecoratorStartVerticalEccentric<T>>(
        this->template f<"Set"_f, "V"_f>(v));
  }
};

template <typename T>
class JESSICA_DLL_PUBLIC DecoratorEndVerticalEccentric : public DecoratorEnd<T>
{
 public:
  using RootType = T;

  template <typename... Args>
  explicit DecoratorEndVerticalEccentric(std::shared_ptr<T>& impl,
                                         Args&&... args)
      : DecoratorEnd<T>(impl, std::forward<Args>(args)...)
  {
  }
  DecoratorEndVerticalEccentric(const DecoratorEndVerticalEccentric&) = default;
  DecoratorEndVerticalEccentric(DecoratorEndVerticalEccentric&&) = delete;
  DecoratorEndVerticalEccentric&
      operator=(const DecoratorEndVerticalEccentric&) = delete;
  DecoratorEndVerticalEccentric&
      operator=(DecoratorEndVerticalEccentric&&) = delete;

  ~DecoratorEndVerticalEccentric() override = default;

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "E"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.E(); }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "E"_f>
  [[nodiscard]] auto f(const T& classe, const double e) const
  {
    return classe.SetE(e);
  }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "V"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.V(); }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "V"_f>
  [[nodiscard]] auto f(const T& classe, const double v) const
  {
    return classe.SetV(v);
  }
};
}  // namespace jessica

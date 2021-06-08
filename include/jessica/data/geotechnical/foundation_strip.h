#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
class JESSICA_DLL_PUBLIC FoundationStrip final
{
 public:
  FoundationStrip() = default;
  FoundationStrip(const FoundationStrip&) = default;
  FoundationStrip(FoundationStrip&&) = delete;
  FoundationStrip& operator=(const FoundationStrip&) = delete;
  FoundationStrip& operator=(FoundationStrip&&) = delete;

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Clone>
  [[nodiscard]] std::shared_ptr<FoundationStrip> f() const
  {
    return std::make_shared<FoundationStrip>(*this);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::B>
  [[nodiscard]] double f() const { return b_; }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::B>
  [[nodiscard]] std::shared_ptr<FoundationStrip> f(const double b) const
  {
    auto retval = f<F::Set, F::Clone>();
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};

template <typename T>
class JESSICA_DLL_PUBLIC FoundationStripDecorator final
{
 public:
  FoundationStripDecorator() = default;
  FoundationStripDecorator(const FoundationStripDecorator&) = default;
  FoundationStripDecorator(FoundationStripDecorator&&) = delete;
  FoundationStripDecorator& operator=(const FoundationStripDecorator&) = delete;
  FoundationStripDecorator& operator=(FoundationStripDecorator&&) = delete;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (Action == F::Set)
    {
      auto retval = f<F::Set, F::Clone>();
      retval->impl_ =
          impl_->template f<Action, U...>(std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return impl_->template f<Action, U...>(std::forward<const Args>(args)...);
    }
  }

  template <F Action, F U>
  requires Equals<F, Action, F::Set> && Equals<F, U, F::Clone>
  [[nodiscard]] std::shared_ptr<FoundationStripDecorator<T>> f() const
  {
    return std::make_shared<FoundationStripDecorator<T>>(*this);
  }

 private:
  std::shared_ptr<FoundationStrip> impl_ = std::make_shared<FoundationStrip>();
};
}  // namespace jessica

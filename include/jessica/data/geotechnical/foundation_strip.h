#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
class JESSICA_DLL_PUBLIC FoundationStripImpl
{
 public:
  FoundationStripImpl() = default;
  FoundationStripImpl(const FoundationStripImpl&) = default;
  FoundationStripImpl(FoundationStripImpl&&) = delete;
  FoundationStripImpl& operator=(const FoundationStripImpl&) = delete;
  FoundationStripImpl& operator=(FoundationStripImpl&&) = delete;

  template <F T>
  requires Equals<F, T, F::Clone>
  [[nodiscard]] static std::shared_ptr<FoundationStripImpl>
      f(const FoundationStripImpl& self)
  {
    return std::make_shared<FoundationStripImpl>(self);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::B>
  [[nodiscard]] static double f(const FoundationStripImpl& self)
  {
    return self.b_;
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::B>
  [[nodiscard]] static std::shared_ptr<FoundationStripImpl>
      f(const FoundationStripImpl& self, const double b)
  {
    auto retval = f<F::Clone>(self);
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};

template <typename T>
class JESSICA_DLL_PUBLIC FoundationStrip final
{
 public:
  FoundationStrip() = default;
  FoundationStrip(const FoundationStrip&) = default;
  FoundationStrip(FoundationStrip&&) = delete;
  FoundationStrip& operator=(const FoundationStrip&) = delete;
  FoundationStrip& operator=(FoundationStrip&&) = delete;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (Action == F::Set)
    {
      auto retval = Clone();
      retval->impl_ = T::template f<Action, U...>(
          *impl_, std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return T::template f<Action, U...>(*impl_,
                                         std::forward<const Args>(args)...);
    }
  }

  [[nodiscard]] std::shared_ptr<FoundationStrip<T>> Clone() const
  {
    return std::make_shared<FoundationStrip<T>>(*this);
  }

 private:
  std::shared_ptr<FoundationStripImpl> impl_ =
      std::make_shared<FoundationStripImpl>();
};
}  // namespace jessica

#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
class JESSICA_DLL_PUBLIC VerticalEccentricImpl
{
 public:
  VerticalEccentricImpl() = default;
  VerticalEccentricImpl(const VerticalEccentricImpl&) = default;
  VerticalEccentricImpl(VerticalEccentricImpl&&) = delete;
  VerticalEccentricImpl& operator=(const VerticalEccentricImpl&) = delete;
  VerticalEccentricImpl& operator=(VerticalEccentricImpl&&) = delete;

  template <F T>
  requires Equals<F, T, F::Clone>
  [[nodiscard]] static std::shared_ptr<VerticalEccentricImpl>
      f(const VerticalEccentricImpl& self)
  {
    return std::make_shared<VerticalEccentricImpl>(self);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::V>
  [[nodiscard]] static double f(const VerticalEccentricImpl& self)
  {
    return self.v_;
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::V>
  [[nodiscard]] static std::shared_ptr<VerticalEccentricImpl>
      f(const VerticalEccentricImpl& self, const double v)
  {
    auto retval = f<F::Clone>(self);
    retval->v_ = v;
    return retval;
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::E>
  [[nodiscard]] static double f(const VerticalEccentricImpl& self)
  {
    return self.e_;
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::E>
  [[nodiscard]] static std::shared_ptr<VerticalEccentricImpl>
      f(const VerticalEccentricImpl& self, const double e)
  {
    auto retval = f<F::Clone>(self);
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};

template <typename T>
class JESSICA_DLL_PUBLIC VerticalEccentric final
{
 public:
  VerticalEccentric() = default;
  VerticalEccentric(const VerticalEccentric&) = default;
  VerticalEccentric(VerticalEccentric&&) = delete;
  VerticalEccentric& operator=(const VerticalEccentric&) = delete;
  VerticalEccentric& operator=(VerticalEccentric&&) = delete;

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

  [[nodiscard]] std::shared_ptr<VerticalEccentric<T>> Clone() const
  {
    return std::make_shared<VerticalEccentric<T>>(*this);
  }

 private:
  std::shared_ptr<VerticalEccentricImpl> impl_ =
      std::make_shared<VerticalEccentricImpl>();
};
}  // namespace jessica

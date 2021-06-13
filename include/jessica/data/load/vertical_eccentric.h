#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
class JESSICA_DLL_PUBLIC VerticalEccentric final
{
 public:
  VerticalEccentric() = default;
  VerticalEccentric(const VerticalEccentric&) = default;
  VerticalEccentric(VerticalEccentric&&) = delete;
  VerticalEccentric& operator=(const VerticalEccentric&) = delete;
  VerticalEccentric& operator=(VerticalEccentric&&) = delete;

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Clone>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f() const
  {
    return std::make_shared<VerticalEccentric>(*this);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::V>
  [[nodiscard]] double f() const { return v_; }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::V>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f(const double v) const
  {
    auto retval = f<F::Set, F::Clone>();
    retval->v_ = v;
    return retval;
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::E>
  [[nodiscard]] double f() const { return e_; }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::E>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f(const double e) const
  {
    auto retval = f<F::Set, F::Clone>();
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

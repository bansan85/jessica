#pragma once

#include <cstdint>
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

  ~VerticalEccentric() = default;

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "Clone"_f>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f() const
  {
    return std::make_shared<VerticalEccentric>(*this);
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "V"_f>
  [[nodiscard]] double f() const { return v_; }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "V"_f>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f(const double v) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->v_ = v;
    return retval;
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "E"_f>
  [[nodiscard]] double f() const { return e_; }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "E"_f>
  [[nodiscard]] std::shared_ptr<VerticalEccentric> f(const double e) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

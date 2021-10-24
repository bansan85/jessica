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

  [[nodiscard]] std::shared_ptr<VerticalEccentric> Clone() const
  {
    return std::make_shared<VerticalEccentric>(*this);
  }

  [[nodiscard]] double V() const { return v_; }

  [[nodiscard]] std::shared_ptr<VerticalEccentric> SetV(const double v) const
  {
    auto retval = Clone();
    retval->v_ = v;
    return retval;
  }

  [[nodiscard]] double E() const { return e_; }

  [[nodiscard]] std::shared_ptr<VerticalEccentric> SetE(const double e) const
  {
    auto retval = Clone();
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

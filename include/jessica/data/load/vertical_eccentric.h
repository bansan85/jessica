#pragma once

#include <cstdint>
#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/macro.h>

namespace jessica
{
class JESSICA_DLL_PUBLIC VerticalEccentric final
{
 public:
  VerticalEccentric() = default;
  RULE_OF_FIVE_COPY_AND_CLONE(VerticalEccentric)

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

  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(cereal::make_nvp("v", v_), cereal::make_nvp("e", e_));
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

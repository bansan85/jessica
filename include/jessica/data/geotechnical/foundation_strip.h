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
class JESSICA_DLL_PUBLIC FoundationStrip final
{
 public:
  FoundationStrip() = default;
  RULE_OF_FIVE_COPY_AND_CLONE(FoundationStrip)

  [[nodiscard]] double B() const { return b_; }

  [[nodiscard]] std::shared_ptr<FoundationStrip> SetB(const double b) const
  {
    auto retval = Clone();
    retval->b_ = b;
    return retval;
  }

  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(cereal::make_nvp("b", b_));
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

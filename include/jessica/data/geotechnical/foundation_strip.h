#pragma once

#include <cstdint>
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

  ~FoundationStrip() = default;

  [[nodiscard]] std::shared_ptr<FoundationStrip> Clone() const
  {
    return std::make_shared<FoundationStrip>(*this);
  }

  [[nodiscard]] double B() const { return b_; }

  [[nodiscard]] std::shared_ptr<FoundationStrip> SetB(const double b) const
  {
    auto retval = Clone();
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

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

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "Clone"_f>
  [[nodiscard]] std::shared_ptr<FoundationStrip> f() const
  {
    return std::make_shared<FoundationStrip>(*this);
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "B"_f>
  [[nodiscard]] double f() const { return b_; }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "B"_f>
  [[nodiscard]] std::shared_ptr<FoundationStrip> f(const double b) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace jessica

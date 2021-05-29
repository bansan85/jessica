#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace Jessica::Data::Geotechnical
{
class JESSICA_DLL_PUBLIC FoundationStripImpl
{
 public:
  FoundationStripImpl() = default;
  FoundationStripImpl(const FoundationStripImpl&) = default;
  FoundationStripImpl(FoundationStripImpl&&) = delete;
  FoundationStripImpl& operator=(const FoundationStripImpl&) = delete;
  FoundationStripImpl& operator=(FoundationStripImpl&&) = delete;

  template <Jessica::Helper::F T>
  requires std::is_same_v<
      std::integral_constant<Jessica::Helper::F, T>,
      std::integral_constant<Jessica::Helper::F, Jessica::Helper::F::Clone>>
  [[nodiscard]] static std::shared_ptr<FoundationStripImpl>
      f(const FoundationStripImpl& self)
  {
    return std::make_shared<FoundationStripImpl>(self);
  }

  template <bool CloneT, Jessica::Helper::F T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type> &&
      std::is_same_v<
          std::integral_constant<Jessica::Helper::F, T>,
          std::integral_constant<Jessica::Helper::F, Jessica::Helper::F::B>>
  [[nodiscard]] static double f(const FoundationStripImpl& self)
  {
    return self.b_;
  }

  template <bool CloneT, Jessica::Helper::F T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::true_type> &&
      std::is_same_v<
          std::integral_constant<Jessica::Helper::F, T>,
          std::integral_constant<Jessica::Helper::F, Jessica::Helper::F::B>>
  [[nodiscard]] static std::shared_ptr<FoundationStripImpl>
      f(const FoundationStripImpl& self, const double b)
  {
    auto retval = f<Jessica::Helper::F::Clone>(self);
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

  template <bool CloneT, Jessica::Helper::F... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (CloneT)
    {
      auto retval = Clone();
      retval->impl_ = T::template f<CloneT, U...>(
          *impl_, std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return T::template f<CloneT, U...>(*impl_,
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
}  // namespace Jessica::Data::Geotechnical

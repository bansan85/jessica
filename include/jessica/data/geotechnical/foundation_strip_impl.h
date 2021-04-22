#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/helper/template.h>

namespace Jessica::Data::Geotechnical
{
class JESSICA_DLL_PUBLIC FoundationStripImpl
{
 public:
  struct Clone
  {
    using ReturnType = std::shared_ptr<FoundationStripImpl>;
  };

  struct GetB
  {
    using ReturnType = double;
  };

  struct SetB
  {
    using ReturnType = std::shared_ptr<FoundationStripImpl>;
    double b;
  };

  FoundationStripImpl() = default;
  FoundationStripImpl(const FoundationStripImpl&) = default;
  FoundationStripImpl(FoundationStripImpl&&) = delete;
  FoundationStripImpl& operator=(const FoundationStripImpl&) = delete;
  FoundationStripImpl& operator=(FoundationStripImpl&&) = delete;

  template <typename T>
  static typename T::ReturnType f(const FoundationStripImpl&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

  template <typename T>
  static typename T::ReturnType f(const FoundationStripImpl&, const T&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};

template <>
[[nodiscard]] std::shared_ptr<FoundationStripImpl>
FoundationStripImpl::f<FoundationStripImpl::Clone>(
    const FoundationStripImpl& self)
{
  return std::make_shared<FoundationStripImpl>(self);
}

template <>
[[nodiscard]] double FoundationStripImpl::f<FoundationStripImpl::GetB>(
    const FoundationStripImpl& self)
{
  return self.b_;
}

template <>
[[nodiscard]] std::shared_ptr<FoundationStripImpl> FoundationStripImpl::f<>(
    const FoundationStripImpl& self, const SetB& b)
{
  auto retval = std::make_shared<FoundationStripImpl>(self);
  retval->b_ = b.b;
  return retval;
}

template <typename T>
class JESSICA_DLL_PUBLIC FoundationStrip final
    : public Jessica::Data::Geotechnical::IFoundationStrip
{
 public:
  FoundationStrip() = default;
  FoundationStrip(const FoundationStrip&) = default;
  FoundationStrip(FoundationStrip&&) = delete;
  FoundationStrip& operator=(const FoundationStrip&) = delete;
  FoundationStrip& operator=(FoundationStrip&&) = delete;
  ~FoundationStrip() override = default;

  [[nodiscard]] std::shared_ptr<IFoundationStrip> Clone() const override
  {
    return std::make_shared<FoundationStrip>(*this);
  }

  [[nodiscard]] double B() const override
  {
    return T::template f<FoundationStripImpl::GetB>(*impl_);
  }

  [[nodiscard]] std::shared_ptr<IFoundationStrip> B(double b) const override
  {
    auto retval = std::make_shared<FoundationStrip>(*this);
    retval->impl_ =
        T::template f(*retval->impl_, FoundationStripImpl::SetB{.b = b});
    return retval;
  }

 private:
  std::shared_ptr<FoundationStripImpl> impl_ =
      std::make_shared<FoundationStripImpl>();
};
}  // namespace Jessica::Data::Geotechnical

#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/template.h>

namespace Jessica::Data::Load
{
class JESSICA_DLL_PUBLIC VerticalEccentricImpl
{
 public:
  struct Clone
  {
    using ReturnType = std::shared_ptr<VerticalEccentricImpl>;
  };

  struct GetV
  {
    using ReturnType = double;
  };

  struct SetV
  {
    using ReturnType = std::shared_ptr<VerticalEccentricImpl>;
    double v;
  };

  struct GetE
  {
    using ReturnType = double;
  };

  struct SetE
  {
    using ReturnType = std::shared_ptr<VerticalEccentricImpl>;
    double e;
  };

  VerticalEccentricImpl() = default;
  VerticalEccentricImpl(const VerticalEccentricImpl&) = default;
  VerticalEccentricImpl(VerticalEccentricImpl&&) = delete;
  VerticalEccentricImpl& operator=(const VerticalEccentricImpl&) = delete;
  VerticalEccentricImpl& operator=(VerticalEccentricImpl&&) = delete;

  template <typename T>
  static typename T::ReturnType f(const VerticalEccentricImpl&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

  template <typename T>
  static typename T::ReturnType f(const VerticalEccentricImpl&, const T&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};

template <>
[[nodiscard]] std::shared_ptr<VerticalEccentricImpl>
VerticalEccentricImpl::f<VerticalEccentricImpl::Clone>(
    const VerticalEccentricImpl& self)
{
  return std::make_shared<VerticalEccentricImpl>(self);
}

template <>
[[nodiscard]] double VerticalEccentricImpl::f<VerticalEccentricImpl::GetV>(
    const VerticalEccentricImpl& self)
{
  return self.v_;
}

template <>
[[nodiscard]] std::shared_ptr<VerticalEccentricImpl> VerticalEccentricImpl::f(
    const VerticalEccentricImpl& self, const SetV& a)
{
  auto retval = std::make_shared<VerticalEccentricImpl>(self);
  retval->v_ = a.v;
  return retval;
}

template <>
[[nodiscard]] double VerticalEccentricImpl::f<VerticalEccentricImpl::GetE>(
    const VerticalEccentricImpl& self)
{
  return self.e_;
}

template <>
[[nodiscard]] std::shared_ptr<VerticalEccentricImpl> VerticalEccentricImpl::f(
    const VerticalEccentricImpl& self, const SetE& a)
{
  auto retval = std::make_shared<VerticalEccentricImpl>(self);
  retval->e_ = a.e;
  return retval;
}

template <typename T>
class JESSICA_DLL_PUBLIC VerticalEccentric final
    : public Jessica::Data::Load::IVerticalEccentric
{
 public:
  VerticalEccentric() = default;
  VerticalEccentric(const VerticalEccentric&) = default;
  VerticalEccentric(VerticalEccentric&&) = delete;
  VerticalEccentric& operator=(const VerticalEccentric&) = delete;
  VerticalEccentric& operator=(VerticalEccentric&&) = delete;
  ~VerticalEccentric() override = default;

  [[nodiscard]] std::shared_ptr<IVerticalEccentric> Clone() const override
  {
    return std::make_shared<VerticalEccentric>(*this);
  }

  [[nodiscard]] double V() const override
  {
    return T::template f<VerticalEccentricImpl::GetV>(*impl_);
  }

  [[nodiscard]] double E() const override
  {
    return T::template f<VerticalEccentricImpl::GetE>(*impl_);
  }

  [[nodiscard]] std::shared_ptr<IVerticalEccentric> V(double v) const override
  {
    auto retval = std::make_shared<VerticalEccentric>(*this);
    retval->impl_ =
        T::template f(*retval->impl_, VerticalEccentricImpl::SetV{.v = v});
    return retval;
  }

  [[nodiscard]] std::shared_ptr<IVerticalEccentric> E(double e) const override
  {
    auto retval = std::make_shared<VerticalEccentric>(*this);
    retval->impl_ =
        T::template f(*retval->impl_, VerticalEccentricImpl::SetE{.e = e});
    return retval;
  }

 private:
  std::shared_ptr<VerticalEccentricImpl> impl_ =
      std::make_shared<VerticalEccentricImpl>();
};
}  // namespace Jessica::Data::Load

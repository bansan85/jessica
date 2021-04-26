#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/template.h>

namespace Jessica::Calc::Geotechnical
{
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundationImpl
{
 public:
  struct Clone
  {
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl>;
  };

  struct GetQref
  {
    using ReturnType = double;
  };

  struct GetB_
  {
    using ReturnType = double;
  };

  MeyerhofShallowFoundationImpl(
      const std::shared_ptr<Data::Load::IVerticalEccentric>& load,
      const std::shared_ptr<Data::Geotechnical::IFoundationStrip>& foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundationImpl(const MeyerhofShallowFoundationImpl&) = default;
  MeyerhofShallowFoundationImpl(MeyerhofShallowFoundationImpl&&) = delete;
  MeyerhofShallowFoundationImpl& operator=(
      const MeyerhofShallowFoundationImpl&) = delete;
  MeyerhofShallowFoundationImpl& operator=(MeyerhofShallowFoundationImpl&&) =
      delete;

  template <typename T>
  static typename T::ReturnType f(const MeyerhofShallowFoundationImpl&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

  template <typename T>
  static typename T::ReturnType f(const MeyerhofShallowFoundationImpl&,
                                  const T&)
  {
    static_assert(Jessica::Helper::StaticAssert<T>::value, "Must specialized");
  }

 private:
  const std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>& load_;
  const std::shared_ptr<Jessica::Data::Geotechnical::IFoundationStrip>&
      foundation_;
};

template <>
[[nodiscard]] std::shared_ptr<MeyerhofShallowFoundationImpl>
MeyerhofShallowFoundationImpl::f<MeyerhofShallowFoundationImpl::Clone>(
    const MeyerhofShallowFoundationImpl& self)
{
  return std::make_shared<MeyerhofShallowFoundationImpl>(self);
}

template <>
[[nodiscard]] double
MeyerhofShallowFoundationImpl::f<MeyerhofShallowFoundationImpl::GetB_>(
    const MeyerhofShallowFoundationImpl& self)
{
  return self.foundation_->B() - 2. * self.load_->E();
}

template <>
[[nodiscard]] double
MeyerhofShallowFoundationImpl::f<MeyerhofShallowFoundationImpl::GetQref>(
    const MeyerhofShallowFoundationImpl& self)
{
  return self.load_->V() /
         MeyerhofShallowFoundationImpl::f<MeyerhofShallowFoundationImpl::GetB_>(
             self);
}

template <typename T>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
    : public Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation
{
 public:
  MeyerhofShallowFoundation(
      const std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>& load,
      const std::shared_ptr<Jessica::Data::Geotechnical::IFoundationStrip>&
          foundation)
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl>(load, foundation))
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation& operator=(const MeyerhofShallowFoundation&) =
      delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;
  ~MeyerhofShallowFoundation() override = default;

  [[nodiscard]] std::shared_ptr<IMeyerhofShallowFoundation> Clone()
      const override
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  [[nodiscard]] double Qref() const override
  {
    return T::template f<MeyerhofShallowFoundationImpl::GetQref>(*impl_);
  }

  [[nodiscard]] double B_() const override
  {
    return T::template f<MeyerhofShallowFoundationImpl::GetB_>(*impl_);
  }

 private:
  std::shared_ptr<MeyerhofShallowFoundationImpl> impl_;
};
}  // namespace Jessica::Calc::Geotechnical

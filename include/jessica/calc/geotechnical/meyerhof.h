#pragma once

#include <memory>

#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace Jessica::Calc::Geotechnical
{
template <typename T0, typename T1>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundationImpl
{
 public:
  MeyerhofShallowFoundationImpl(
      const std::shared_ptr<Data::Load::VerticalEccentric<T0>>& load,
      const std::shared_ptr<Data::Geotechnical::FoundationStrip<T1>>&
          foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundationImpl(const MeyerhofShallowFoundationImpl&) = default;
  MeyerhofShallowFoundationImpl(MeyerhofShallowFoundationImpl&&) = delete;
  MeyerhofShallowFoundationImpl& operator=(
      const MeyerhofShallowFoundationImpl&) = delete;
  MeyerhofShallowFoundationImpl& operator=(MeyerhofShallowFoundationImpl&&) =
      delete;

  template <Jessica::Helper::F T>
  requires std::is_same_v<
      std::integral_constant<Jessica::Helper::F, T>,
      std::integral_constant<
          Jessica::Helper::F,
          Jessica::Helper::F::Clone>> [[nodiscard]] static std::
      shared_ptr<MeyerhofShallowFoundationImpl>
      f(const MeyerhofShallowFoundationImpl& self)
  {
    return std::make_shared<MeyerhofShallowFoundationImpl>(self);
  }

  template <bool CloneT, Jessica::Helper::F T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&&
      std::is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                     std::integral_constant<
                         Jessica::Helper::F,
                         Jessica::Helper::F::B_>> [[nodiscard]] static double
      f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.foundation_->template f<false, Jessica::Helper::F::B>() -
           2. * self.load_->template f<false, Jessica::Helper::F::E>();
  }

  template <bool CloneT, Jessica::Helper::F T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&&
      std::is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                     std::integral_constant<
                         Jessica::Helper::F,
                         Jessica::Helper::F::Qref>> [[nodiscard]] static double
      f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.load_->template f<false, Jessica::Helper::F::V>() /
           f<false, Jessica::Helper::F::B_>(self);
  }

  template <bool CloneT, Jessica::Helper::F T, Jessica::Helper::F... U,
            typename... Args>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&&
      std::is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                     std::integral_constant<
                         Jessica::Helper::F,
                         Jessica::Helper::F::Load>> [[nodiscard]] static double
      f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    return self.load_->template f<CloneT, U...>(
        std::forward<const Args>(args)...);
  }

  template <bool CloneT, Jessica::Helper::F T, Jessica::Helper::F... U,
            typename... Args>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::true_type>&&
      std::is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                     std::integral_constant<
                         Jessica::Helper::F,
                         Jessica::Helper::F::Load>> [[nodiscard]] static std::
          shared_ptr<MeyerhofShallowFoundationImpl>
          f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    auto retval = f<Jessica::Helper::F::Clone>(self);
    retval->load_ = retval->load_->template f<CloneT, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

  template <bool CloneT, Jessica::Helper::F T, Jessica::Helper::F... U,
            typename... Args>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&& std::
      is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                std::integral_constant<
                    Jessica::Helper::F,
                    Jessica::Helper::F::Foundation>> [[nodiscard]] static double
      f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    return self.foundation_->template f<CloneT, U...>(
        std::forward<const Args>(args)...);
  }

  template <bool CloneT, Jessica::Helper::F T, Jessica::Helper::F... U,
            typename... Args>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::true_type>&& std::
      is_same_v<std::integral_constant<Jessica::Helper::F, T>,
                std::integral_constant<
                    Jessica::Helper::F,
                    Jessica::Helper::F::Foundation>> [[nodiscard]] static std::
          shared_ptr<MeyerhofShallowFoundationImpl>
          f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    auto retval = f<Jessica::Helper::F::Clone>(self);
    retval->foundation_ = retval->foundation_->template f<CloneT, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

 private:
  std::shared_ptr<Jessica::Data::Load::VerticalEccentric<T0>> load_;
  std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<T1>> foundation_;
};

template <typename T>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  using T0 = typename Jessica::Helper::ExtractRootTypeN<0, T>::type;
  using T1 = typename Jessica::Helper::ExtractRootTypeN<1, T>::type;

  MeyerhofShallowFoundation()
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<T0, T1>>(
            std::make_shared<Jessica::Data::Load::VerticalEccentric<T0>>(),
            std::make_shared<
                Jessica::Data::Geotechnical::FoundationStrip<T1>>()))
  {
  }
  MeyerhofShallowFoundation(
      const std::shared_ptr<Jessica::Data::Load::VerticalEccentric<T0>>& load,
      const std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<T1>>&
          foundation)
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<T0, T1>>(
            load, foundation))
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation& operator=(const MeyerhofShallowFoundation&) =
      delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;
  ~MeyerhofShallowFoundation() = default;

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

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> Clone() const
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

 private:
  std::shared_ptr<MeyerhofShallowFoundationImpl<T0, T1>> impl_;
};
}  // namespace Jessica::Calc::Geotechnical

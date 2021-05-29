#pragma once

#include <memory>

#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <typename T0, typename T1>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundationImpl
{
 public:
  MeyerhofShallowFoundationImpl(
      const std::shared_ptr<VerticalEccentric<T0>>& load,
      const std::shared_ptr<FoundationStrip<T1>>& foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundationImpl(const MeyerhofShallowFoundationImpl&) = default;
  MeyerhofShallowFoundationImpl(MeyerhofShallowFoundationImpl&&) = delete;
  MeyerhofShallowFoundationImpl&
      operator=(const MeyerhofShallowFoundationImpl&) = delete;
  MeyerhofShallowFoundationImpl&
      operator=(MeyerhofShallowFoundationImpl&&) = delete;

  template <F T>
  requires Equals<F, T, F::Clone>
  [[nodiscard]] static std::shared_ptr<MeyerhofShallowFoundationImpl>
      f(const MeyerhofShallowFoundationImpl& self)
  {
    return std::make_shared<MeyerhofShallowFoundationImpl>(self);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::B_>
  [[nodiscard]] static double f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.foundation_->template f<F::Get, F::B>() -
           2. * self.load_->template f<F::Get, F::E>();
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Qref>
  [[nodiscard]] static double f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.load_->template f<F::Get, F::V>() / f<F::Get, F::B_>(self);
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Load>
  [[nodiscard]] static double f(const MeyerhofShallowFoundationImpl& self,
                                const Args&&... args)
  {
    return self.load_->template f<Action, U...>(
        std::forward<const Args>(args)...);
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Load>
  [[nodiscard]] static std::shared_ptr<MeyerhofShallowFoundationImpl>
      f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    auto retval = f<F::Clone>(self);
    retval->load_ = retval->load_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Foundation>
  [[nodiscard]] static double f(const MeyerhofShallowFoundationImpl& self,
                                const Args&&... args)
  {
    return self.foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Foundation>
  [[nodiscard]] static std::shared_ptr<MeyerhofShallowFoundationImpl>
      f(const MeyerhofShallowFoundationImpl& self, const Args&&... args)
  {
    auto retval = f<F::Clone>(self);
    retval->foundation_ = retval->foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

 private:
  std::shared_ptr<VerticalEccentric<T0>> load_;
  std::shared_ptr<FoundationStrip<T1>> foundation_;
};

template <typename T>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  using T0 = typename ExtractRootTypeN<0, T>::type;
  using T1 = typename ExtractRootTypeN<1, T>::type;

  MeyerhofShallowFoundation()
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<T0, T1>>(
            std::make_shared<VerticalEccentric<T0>>(),
            std::make_shared<FoundationStrip<T1>>()))
  {
  }
  MeyerhofShallowFoundation(
      const std::shared_ptr<VerticalEccentric<T0>>& load,
      const std::shared_ptr<FoundationStrip<T1>>& foundation)
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<T0, T1>>(
            load, foundation))
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation&
      operator=(const MeyerhofShallowFoundation&) = delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;
  ~MeyerhofShallowFoundation() = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (Action == F::Set)
    {
      auto retval = Clone();
      retval->impl_ = T::template f<Action, U...>(
          *impl_, std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return T::template f<Action, U...>(*impl_,
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
}  // namespace jessica

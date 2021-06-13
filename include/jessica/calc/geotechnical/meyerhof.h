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
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  MeyerhofShallowFoundation(const std::shared_ptr<T0>& load,
                            const std::shared_ptr<T1>& foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation&
      operator=(const MeyerhofShallowFoundation&) = delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Clone>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> f() const
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::B_>
  [[nodiscard]] double f() const
  {
    return foundation_->template f<F::Get, F::B>() -
           2. * load_->template f<F::Get, F::E>();
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Qref>
  [[nodiscard]] double f() const
  {
    return load_->template f<F::Get, F::V>() / f<F::Get, F::B_>();
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Load>
  [[nodiscard]] double f(const Args&&... args) const
  {
    return load_->template f<Action, U...>(std::forward<const Args>(args)...);
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Load>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      f(const Args&&... args) const
  {
    auto retval = f<F::Set, F::Clone>();
    retval->load_ = retval->load_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::Foundation>
  [[nodiscard]] double f(const Args&&... args) const
  {
    return foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
  }

  template <F Action, F T, F... U, typename... Args>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Foundation>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      f(const Args&&... args) const
  {
    auto retval = f<F::Set, F::Clone>();
    retval->foundation_ = retval->foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

 private:
  std::shared_ptr<T0> load_;
  std::shared_ptr<T1> foundation_;
};
}  // namespace jessica

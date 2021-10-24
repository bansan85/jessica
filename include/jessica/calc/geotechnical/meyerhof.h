#pragma once

#include <cstdint>
#include <memory>

#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>

namespace jessica
{
template <class Type>
struct S;

template <typename T0, typename T1>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  MeyerhofShallowFoundation(std::shared_ptr<T0> load,
                            std::shared_ptr<T1> foundation)
      : load_(std::move(load)), foundation_(std::move(foundation))
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation&
      operator=(const MeyerhofShallowFoundation&) = delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;

  ~MeyerhofShallowFoundation() = default;

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> Clone() const
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  [[nodiscard]] double B_() const { return foundation_->B() - 2. * load_->E(); }

  [[nodiscard]] double Qref() const { return load_->V() / B_(); }

  template <typename Fct, typename... Args>
  [[nodiscard]] auto Foundation(const Fct fct, Args&&... args) const
  {
    return (foundation_.get()->*fct)(std::forward<Args>(args)...);
  }

  template <typename Fct, typename... Args>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      SetFoundation(const Fct fct, Args&&... args) const
  {
    auto retval = Clone();
    // cppcheck-suppress redundantAssignment
    retval->foundation_ =
        (retval->foundation_.get()->*fct)(std::forward<Args>(args)...);
    return retval;
  }

  template <typename Fct, typename... Args>
  [[nodiscard]] auto Load(const Fct fct, Args&&... args) const
  {
    return (load_.get()->*fct)(std::forward<Args>(args)...);
  }

  template <typename Fct, typename... Args>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      SetLoad(const Fct fct, Args&&... args) const
  {
    auto retval = Clone();
    // cppcheck-suppress redundantAssignment
    retval->load_ = (retval->load_.get()->*fct)(std::forward<Args>(args)...);
    return retval;
  }

 private:
  std::shared_ptr<T0> load_;
  std::shared_ptr<T1> foundation_;
};
}  // namespace jessica

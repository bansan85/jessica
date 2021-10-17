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

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "Clone"_f>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> f() const
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "B_"_f>
  [[nodiscard]] double f() const
  {
    return foundation_->template f<Action, "B"_f>() -
           2. * load_->template f<Action, "E"_f>();
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "Qref"_f>
  [[nodiscard]] double f() const
  {
    return load_->template f<Action, "V"_f>() / f<Action, "B_"_f>();
  }

  template <uint64_t Action, uint64_t T, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "Load"_f>
  [[nodiscard]] double f(const Args&&... args) const
  {
    return load_->template f<Action, U...>(std::forward<const Args>(args)...);
  }

  template <uint64_t Action, uint64_t T, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "Load"_f>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      f(const Args&&... args) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->load_ = retval->load_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

  template <uint64_t Action, uint64_t T, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "_foundation"_f>
  [[nodiscard]] double f(const Args&&... args) const
  {
    return foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
  }

  template <uint64_t Action, uint64_t T, uint64_t... U, typename... Args>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "_foundation"_f>
  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation>
      f(const Args&&... args) const
  {
    auto retval = f<Action, "Clone"_f>();
    retval->foundation_ = retval->foundation_->template f<Action, U...>(
        std::forward<const Args>(args)...);
    return retval;
  }

 private:
  std::shared_ptr<T0> load_;
  std::shared_ptr<T1> foundation_;
};
}  // namespace jessica

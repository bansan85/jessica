#pragma once

#include <cstdint>
#include <memory>

#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/macro.h>

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
  RULE_OF_FIVE_COPY_AND_CLONE(MeyerhofShallowFoundation)

  [[nodiscard]] double B_() const { return foundation_->B() - 2. * load_->E(); }

  [[nodiscard]] double Qref() const { return load_->V() / B_(); }

  POCO_GET_FUNCTION(Foundation, foundation_)
  // cppcheck-suppress redundantAssignment
  POCO_SET_FUNCTION(MeyerhofShallowFoundation, Foundation, foundation_)

  POCO_GET_FUNCTION(Load, load_)
  // cppcheck-suppress redundantAssignment
  POCO_SET_FUNCTION(MeyerhofShallowFoundation, Load, load_)

 private:
  std::shared_ptr<T0> load_;
  std::shared_ptr<T1> foundation_;
};
}  // namespace jessica

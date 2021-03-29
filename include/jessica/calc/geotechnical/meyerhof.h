#pragma once

#include <memory>

#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>

namespace Calc::Geotechnical
{
class IMeyerhofShallowFoundation
{
 public:
  IMeyerhofShallowFoundation(IMeyerhofShallowFoundation&&) = delete;
  IMeyerhofShallowFoundation& operator=(const IMeyerhofShallowFoundation&) =
      delete;
  IMeyerhofShallowFoundation& operator=(IMeyerhofShallowFoundation&&) = delete;
  virtual ~IMeyerhofShallowFoundation() = default;

  [[nodiscard]] virtual std::shared_ptr<IMeyerhofShallowFoundation> Clone()
      const = 0;

  [[nodiscard]] virtual double Qref() const noexcept = 0;
  [[nodiscard]] virtual double B_() const noexcept = 0;

 protected:
  IMeyerhofShallowFoundation() = default;
  IMeyerhofShallowFoundation(const IMeyerhofShallowFoundation&) = default;
};

class Create
{
 public:
  static std::shared_ptr<IMeyerhofShallowFoundation> MeyerhofShallowFoundation(
      const std::shared_ptr<Data::Load::IVerticalEccentric>& load,
      const std::shared_ptr<Data::Geotechnical::IFoundationStrip>& foundation);
};
}  // namespace Calc::Geotechnical

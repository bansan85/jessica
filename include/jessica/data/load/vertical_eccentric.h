#pragma once

#include <memory>

#include <jessica/compat.h>

namespace Jessica::Data::Load
{
class JESSICA_DLL_PUBLIC IVerticalEccentric
{
 public:
  IVerticalEccentric(IVerticalEccentric&&) = delete;
  IVerticalEccentric& operator=(const IVerticalEccentric&) = delete;
  IVerticalEccentric& operator=(IVerticalEccentric&&) = delete;
  virtual ~IVerticalEccentric() = default;

  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> Clone() const = 0;

  [[nodiscard]] virtual double V() const = 0;
  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> V(
      double v) const = 0;
  [[nodiscard]] virtual double E() const = 0;
  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> E(
      double e) const = 0;

 protected:
  IVerticalEccentric() = default;
  IVerticalEccentric(const IVerticalEccentric&) = default;
};
}  // namespace Jessica::Data::Load

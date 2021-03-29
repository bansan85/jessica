#pragma once

#include <memory>

namespace Data::Load
{
class IVerticalEccentric
{
 public:
  IVerticalEccentric(IVerticalEccentric&&) = delete;
  IVerticalEccentric& operator=(const IVerticalEccentric&) = delete;
  IVerticalEccentric& operator=(IVerticalEccentric&&) = delete;
  virtual ~IVerticalEccentric() = default;

  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> Clone() const = 0;

  [[nodiscard]] virtual double V() const noexcept = 0;
  [[nodiscard]] virtual double E() const noexcept = 0;
  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> V(
      double v) const = 0;
  [[nodiscard]] virtual std::shared_ptr<IVerticalEccentric> E(
      double e) const = 0;

 protected:
  IVerticalEccentric() = default;
  IVerticalEccentric(const IVerticalEccentric&) = default;
};

class Create
{
 public:
  static std::shared_ptr<IVerticalEccentric> VerticalEccentric();
};
}  // namespace Data::Load

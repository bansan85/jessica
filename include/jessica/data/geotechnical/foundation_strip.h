#pragma once

#include <memory>

namespace Data::Geotechnical
{
class IFoundationStrip
{
 public:
  IFoundationStrip(IFoundationStrip&&) = delete;
  IFoundationStrip& operator=(const IFoundationStrip&) = delete;
  IFoundationStrip& operator=(IFoundationStrip&&) = delete;
  virtual ~IFoundationStrip() = default;

  [[nodiscard]] virtual std::shared_ptr<IFoundationStrip> Clone() const = 0;

  [[nodiscard]] virtual double B() const noexcept = 0;
  [[nodiscard]] virtual std::shared_ptr<IFoundationStrip> B(double b) = 0;

 protected:
  IFoundationStrip() = default;
  IFoundationStrip(const IFoundationStrip&) = default;
};

class Create
{
 public:
  static std::shared_ptr<IFoundationStrip> FoundationStrip();
};
}  // namespace Data::Geotechnical

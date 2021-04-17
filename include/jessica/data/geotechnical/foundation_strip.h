#pragma once

#include <memory>

#include <jessica/compat.h>

namespace Jessica::Data::Geotechnical
{
class JESSICA_DLL_PUBLIC IFoundationStrip
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

class JESSICA_DLL_PUBLIC Create
{
 public:
  static std::shared_ptr<IFoundationStrip> FoundationStrip();
};
}  // namespace Jessica::Data::Geotechnical

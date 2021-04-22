#pragma once

#include <memory>

#include <jessica/compat.h>

namespace Jessica::Calc::Geotechnical
{
class JESSICA_DLL_PUBLIC IMeyerhofShallowFoundation
{
 public:
  IMeyerhofShallowFoundation(IMeyerhofShallowFoundation&&) = delete;
  IMeyerhofShallowFoundation& operator=(const IMeyerhofShallowFoundation&) =
      delete;
  IMeyerhofShallowFoundation& operator=(IMeyerhofShallowFoundation&&) = delete;
  virtual ~IMeyerhofShallowFoundation() = default;

  [[nodiscard]] virtual std::shared_ptr<IMeyerhofShallowFoundation> Clone()
      const = 0;

  [[nodiscard]] virtual double Qref() const = 0;
  [[nodiscard]] virtual double B_() const = 0;

 protected:
  IMeyerhofShallowFoundation() = default;
  IMeyerhofShallowFoundation(const IMeyerhofShallowFoundation&) = default;
};
}  // namespace Jessica::Calc::Geotechnical

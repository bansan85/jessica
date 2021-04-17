#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>

namespace Jessica::Calc::Geotechnical
{
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
    : public Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation
{
 public:
  MeyerhofShallowFoundation(
      const std::shared_ptr<Data::Load::IVerticalEccentric>& load,
      const std::shared_ptr<Data::Geotechnical::IFoundationStrip>& foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation& operator=(const MeyerhofShallowFoundation&) =
      delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;
  ~MeyerhofShallowFoundation() override = default;

  [[nodiscard]] std::shared_ptr<IMeyerhofShallowFoundation> Clone()
      const override
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  [[nodiscard]] double Qref() const noexcept override
  {
    return load_->V() / B_();
  }
  [[nodiscard]] double B_() const noexcept override
  {
    return foundation_->B() - 2. * load_->E();
  }

 private:
  const std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>& load_;
  const std::shared_ptr<Jessica::Data::Geotechnical::IFoundationStrip>&
      foundation_;
};

std::shared_ptr<IMeyerhofShallowFoundation>
Geotechnical::Create::MeyerhofShallowFoundation(
    const std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>& load,
    const std::shared_ptr<Jessica::Data::Geotechnical::IFoundationStrip>&
        foundation)
{
  return std::make_shared<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation>(load, foundation);
}
}  // namespace Jessica::Calc::Geotechnical

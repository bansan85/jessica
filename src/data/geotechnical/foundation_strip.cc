#include <limits>
#include <memory>

#include <jessica/data/geotechnical/foundation_strip.h>

namespace
{
class FoundationStrip : public Data::Geotechnical::IFoundationStrip
{
 public:
  FoundationStrip() = default;
  FoundationStrip(const FoundationStrip&) = default;
  FoundationStrip(FoundationStrip&&) = delete;
  FoundationStrip& operator=(const FoundationStrip&) = delete;
  FoundationStrip& operator=(FoundationStrip&&) = delete;
  ~FoundationStrip() override = default;

  [[nodiscard]] std::shared_ptr<IFoundationStrip> Clone() const override
  {
    return std::make_shared<FoundationStrip>(*this);
  }

  [[nodiscard]] double B() const noexcept override { return b_; }
  [[nodiscard]] std::shared_ptr<IFoundationStrip> B(double b) override
  {
    auto retval = std::make_shared<FoundationStrip>(*this);
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace

namespace Data::Geotechnical
{
std::shared_ptr<IFoundationStrip> Create::FoundationStrip()
{
  return std::make_shared<::FoundationStrip>();
}
}  // namespace Data::Geotechnical

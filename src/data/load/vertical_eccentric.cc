#include <limits>
#include <memory>

#include <jessica/data/load/vertical_eccentric.h>

namespace
{
class VerticalEccentric : public Data::Load::IVerticalEccentric
{
 public:
  VerticalEccentric() = default;
  VerticalEccentric(const VerticalEccentric&) = default;
  VerticalEccentric(VerticalEccentric&&) = delete;
  VerticalEccentric& operator=(const VerticalEccentric&) = delete;
  VerticalEccentric& operator=(VerticalEccentric&&) = delete;
  ~VerticalEccentric() override = default;

  [[nodiscard]] std::shared_ptr<IVerticalEccentric> Clone() const override
  {
    return std::make_shared<VerticalEccentric>(*this);
  }

  [[nodiscard]] double V() const noexcept override { return v_; }
  [[nodiscard]] double E() const noexcept override { return e_; }
  [[nodiscard]] std::shared_ptr<IVerticalEccentric> V(double v) const override
  {
    auto retval = std::make_shared<VerticalEccentric>(*this);
    retval->v_ = v;
    return retval;
  }
  [[nodiscard]] std::shared_ptr<IVerticalEccentric> E(double e) const override
  {
    auto retval = std::make_shared<VerticalEccentric>(*this);
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};
}  // namespace

namespace Data::Load
{
std::shared_ptr<IVerticalEccentric> Create::VerticalEccentric()
{
  return std::make_shared<::VerticalEccentric>();
}
}  // namespace Data::Load

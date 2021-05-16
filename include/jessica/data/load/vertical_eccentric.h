#pragma once

#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/template.h>

namespace Jessica::Data::Load
{
class JESSICA_DLL_PUBLIC VerticalEccentricImpl
{
 public:
  struct Clone
  {
  };

  struct V
  {
  };

  struct E
  {
  };

  VerticalEccentricImpl() = default;
  VerticalEccentricImpl(const VerticalEccentricImpl&) = default;
  VerticalEccentricImpl(VerticalEccentricImpl&&) = delete;
  VerticalEccentricImpl& operator=(const VerticalEccentricImpl&) = delete;
  VerticalEccentricImpl& operator=(VerticalEccentricImpl&&) = delete;

  template <typename T>
  requires std::is_same_v<Clone, T> [[nodiscard]] static std::shared_ptr<
      VerticalEccentricImpl>
  f(const VerticalEccentricImpl& self)
  {
    return std::make_shared<VerticalEccentricImpl>(self);
  }

  template <bool CloneT, typename T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&&
      std::is_same_v<V, T> [[nodiscard]] static double
      f(const VerticalEccentricImpl& self)
  {
    return self.v_;
  }

  template <bool CloneT, typename T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::true_type>&&
      std::is_same_v<V, T> [[nodiscard]] static std::shared_ptr<
          VerticalEccentricImpl>
      f(const VerticalEccentricImpl& self, const double v)
  {
    auto retval = f<Clone>(self);
    retval->v_ = v;
    return retval;
  }

  template <bool CloneT, typename T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::false_type>&&
      std::is_same_v<E, T> [[nodiscard]] static double
      f(const VerticalEccentricImpl& self)
  {
    return self.e_;
  }

  template <bool CloneT, typename T>
  requires std::is_same_v<std::integral_constant<bool, CloneT>,
                          std::true_type>&&
      std::is_same_v<E, T> [[nodiscard]] static std::shared_ptr<
          VerticalEccentricImpl>
      f(const VerticalEccentricImpl& self, const double e)
  {
    auto retval = f<Clone>(self);
    retval->e_ = e;
    return retval;
  }

 private:
  double v_ = std::numeric_limits<double>::quiet_NaN();
  double e_ = std::numeric_limits<double>::quiet_NaN();
};

template <typename T>
class JESSICA_DLL_PUBLIC VerticalEccentric final
{
 public:
  VerticalEccentric() = default;
  VerticalEccentric(const VerticalEccentric&) = default;
  VerticalEccentric(VerticalEccentric&&) = delete;
  VerticalEccentric& operator=(const VerticalEccentric&) = delete;
  VerticalEccentric& operator=(VerticalEccentric&&) = delete;

  template <bool CloneT, typename... U, typename... Args>
  [[nodiscard]] auto f(const Args&&... args) const
  {
    if constexpr (CloneT)
    {
      auto retval = Clone();
      retval->impl_ = T::template f<CloneT, U...>(
          *impl_, std::forward<const Args>(args)...);
      return retval;
    }
    else
    {
      return T::template f<CloneT, U...>(*impl_,
                                         std::forward<const Args>(args)...);
    }
  }

  [[nodiscard]] std::shared_ptr<VerticalEccentric<T>> Clone() const
  {
    return std::make_shared<VerticalEccentric<T>>(*this);
  }

 private:
  std::shared_ptr<VerticalEccentricImpl> impl_ =
      std::make_shared<VerticalEccentricImpl>();
};
}  // namespace Jessica::Data::Load

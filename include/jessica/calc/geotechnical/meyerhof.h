#pragma once

#include <memory>

#include <jessica/calc/geotechnical/meyerhof_concept.h>
#include <jessica/compat.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/template.h>

namespace Jessica::Calc::Geotechnical
{
template <typename U, typename V>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundationImpl
{
 public:
  struct Clone
  {
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl<U, V>>;
  };

  struct GetQref
  {
    using ReturnType = double;
  };

  struct GetB_
  {
    using ReturnType = double;
  };

  MeyerhofShallowFoundationImpl(
      const std::shared_ptr<Data::Load::VerticalEccentric<U>>& load,
      const std::shared_ptr<Data::Geotechnical::FoundationStrip<V>>& foundation)
      : load_(load), foundation_(foundation)
  {
  }
  MeyerhofShallowFoundationImpl(const MeyerhofShallowFoundationImpl&) = default;
  MeyerhofShallowFoundationImpl(MeyerhofShallowFoundationImpl&&) = delete;
  MeyerhofShallowFoundationImpl& operator=(
      const MeyerhofShallowFoundationImpl&) = delete;
  MeyerhofShallowFoundationImpl& operator=(MeyerhofShallowFoundationImpl&&) =
      delete;

  template <typename T>
  [[nodiscard]] static std::enable_if_t<
      std::is_same_v<MeyerhofShallowFoundationImpl<U, V>::Clone, T>,
      typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl<U, V>& self)
  {
    return std::make_shared<MeyerhofShallowFoundationImpl<U, V>>(self);
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<
      std::is_same_v<MeyerhofShallowFoundationImpl<U, V>::GetB_, T>,
      typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl<U, V>& self)
  {
    return self.foundation_->B() - 2. * self.load_->E();
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<
      std::is_same_v<MeyerhofShallowFoundationImpl<U, V>::GetQref, T>,
      typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl<U, V>& self)
  {
    return self.load_->V() /
           f<MeyerhofShallowFoundationImpl<U, V>::GetB_>(self);
  }

 private:
  const std::shared_ptr<Jessica::Data::Load::VerticalEccentric<U>> load_;
  const std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<V>>
      foundation_;
};

template <typename T>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  using U = typename Jessica::Helper::ExtractRootTypeN<0, T>::Root;
  using V = typename Jessica::Helper::ExtractRootTypeN<1, T>::Root;

  MeyerhofShallowFoundation(
      const std::shared_ptr<Jessica::Data::Load::VerticalEccentric<U>>& load,
      const std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<V>>&
          foundation)
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<U, V>>(load,
                                                                    foundation))
  {
  }
  MeyerhofShallowFoundation(const MeyerhofShallowFoundation&) = default;
  MeyerhofShallowFoundation(MeyerhofShallowFoundation&&) = delete;
  MeyerhofShallowFoundation& operator=(const MeyerhofShallowFoundation&) =
      delete;
  MeyerhofShallowFoundation& operator=(MeyerhofShallowFoundation&&) = delete;
  ~MeyerhofShallowFoundation() = default;

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> Clone() const
  {
    return std::make_shared<MeyerhofShallowFoundation>(*this);
  }

  [[nodiscard]] double Qref() const
  {
    return T::template f<typename MeyerhofShallowFoundationImpl<U, V>::GetQref>(
        *impl_);
  }

  [[nodiscard]] double B_() const
  {
    return T::template f<typename MeyerhofShallowFoundationImpl<U, V>::GetB_>(
        *impl_);
  }

 private:
  std::shared_ptr<MeyerhofShallowFoundationImpl<U, V>> impl_;
};

#ifdef JCONCEPTS
static_assert(MeyerhofShallowFoundationConcept<
              MeyerhofShallowFoundation<std::pair<int, int>>>);
#endif

}  // namespace Jessica::Calc::Geotechnical

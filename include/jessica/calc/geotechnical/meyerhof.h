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
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl>;
  };

  struct GetQref
  {
    using ReturnType = double;
  };

  struct GetB_
  {
    using ReturnType = double;
  };

  struct GetLoadV
  {
    using ReturnType = double;
  };

  struct SetLoadV
  {
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl>;
    double v;
  };

  struct GetLoadE
  {
    using ReturnType = double;
  };

  struct SetLoadE
  {
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl>;
    double e;
  };

  struct GetFoundationB
  {
    using ReturnType = double;
  };

  struct SetFoundationB
  {
    using ReturnType = std::shared_ptr<MeyerhofShallowFoundationImpl>;
    double b;
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
  [[nodiscard]] static std::enable_if_t<std::is_same_v<Clone, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return std::make_shared<MeyerhofShallowFoundationImpl>(self);
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<GetB_, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.foundation_->B() - 2. * self.load_->E();
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<GetQref, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.load_->V() / f<GetB_>(self);
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<GetLoadV, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.load_->V();
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<SetLoadV, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self, const T& a)
  {
    auto retval = f<Clone>(self);
    retval->load_ = retval->load_->V(a.v);
    return retval;
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<GetLoadE, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.load_->E();
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<SetLoadE, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self, const T& a)
  {
    auto retval = f<Clone>(self);
    retval->load_ = retval->load_->E(a.e);
    return retval;
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<GetFoundationB, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self)
  {
    return self.foundation_->B();
  }

  template <typename T>
  [[nodiscard]] static std::enable_if_t<std::is_same_v<SetFoundationB, T>,
                                        typename T::ReturnType>
  f(const MeyerhofShallowFoundationImpl& self, const T& a)
  {
    auto retval = f<Clone>(self);
    retval->foundation_ = retval->foundation_->B(a.b);
    return retval;
  }

 private:
  std::shared_ptr<Jessica::Data::Load::VerticalEccentric<U>> load_;
  std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<V>> foundation_;
};

template <typename T>
class JESSICA_DLL_PUBLIC MeyerhofShallowFoundation final
{
 public:
  using U = typename Jessica::Helper::ExtractRootTypeN<0, T>::Root;
  using V = typename Jessica::Helper::ExtractRootTypeN<1, T>::Root;

  MeyerhofShallowFoundation()
      : impl_(std::make_shared<MeyerhofShallowFoundationImpl<U, V>>(
            std::make_shared<Jessica::Data::Load::VerticalEccentric<U>>(),
            std::make_shared<
                Jessica::Data::Geotechnical::FoundationStrip<V>>()))
  {
  }
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

  [[nodiscard]] double LoadV() const
  {
    return T::template f<
        typename MeyerhofShallowFoundationImpl<U, V>::GetLoadV>(*impl_);
  }

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> LoadV(double v) const
  {
    auto retval = Clone();
    retval->impl_ = T::template f(
        *retval->impl_,
        typename MeyerhofShallowFoundationImpl<U, V>::SetLoadV{.v = v});
    return retval;
  }

  [[nodiscard]] double LoadE() const
  {
    return T::template f<
        typename MeyerhofShallowFoundationImpl<U, V>::GetLoadE>(*impl_);
  }

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> LoadE(double e) const
  {
    auto retval = Clone();
    retval->impl_ = T::template f(
        *retval->impl_,
        typename MeyerhofShallowFoundationImpl<U, V>::SetLoadE{.e = e});
    return retval;
  }

  [[nodiscard]] double FoundationB() const
  {
    return T::template f<
        typename MeyerhofShallowFoundationImpl<U, V>::GetFoundationB>(*impl_);
  }

  [[nodiscard]] std::shared_ptr<MeyerhofShallowFoundation> FoundationB(
      double b) const
  {
    auto retval = Clone();
    retval->impl_ = T::template f(
        *retval->impl_,
        typename MeyerhofShallowFoundationImpl<U, V>::SetFoundationB{.b = b});
    return retval;
  }

 private:
  std::shared_ptr<MeyerhofShallowFoundationImpl<U, V>> impl_;
};

#ifdef JCONCEPTS
static_assert(MeyerhofShallowFoundationConcept<
              MeyerhofShallowFoundation<std::pair<int, int>>>);
#endif

}  // namespace Jessica::Calc::Geotechnical

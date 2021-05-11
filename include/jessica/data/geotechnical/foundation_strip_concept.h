#pragma once

#if !defined(JCONCEPTS) && __has_include(<concepts>)
  #define JCONCEPTS
  #include <concepts>
#endif

#ifdef JCONCEPTS
  #include <memory>

namespace Jessica::Data::Geotechnical
{
template <typename T>
concept FoundationStripConcept = requires(const T t)
{
  {
    t.Clone()
  }
  ->std::same_as<std::shared_ptr<T>>;
  {
    t.B()
  }
  ->std::same_as<double>;
  {
    t.B(std::declval<double>())
  }
  ->std::same_as<std::shared_ptr<T>>;
};
}  // namespace Jessica::Data::Geotechnical

#endif

#pragma once

#if !defined(JCONCEPTS) && __has_include(<concepts>)
  #define JCONCEPTS
  #include <concepts>
#endif

#ifdef JCONCEPTS
  #include <memory>

namespace Jessica::Calc::Geotechnical
{
template <typename T>
concept MeyerhofShallowFoundationConcept = requires(const T t)
{
  {
    t.Clone()
  }
  ->std::same_as<std::shared_ptr<T>>;
  {
    t.Qref()
  }
  ->std::same_as<double>;
  {
    t.B_()
  }
  ->std::same_as<double>;
};
}  // namespace Jessica::Calc::Geotechnical

#endif

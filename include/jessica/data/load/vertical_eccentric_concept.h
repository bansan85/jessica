#pragma once

#if !defined(JCONCEPTS) && __has_include(<concepts>)
  #define JCONCEPTS
  #include <concepts>
#endif

#ifdef JCONCEPTS
  #include <memory>

namespace Jessica::Data::Load
{
template <typename T>
concept VerticalEccentricConcept = requires(const T t)
{
  {
    t.Clone()
  }
  ->std::same_as<std::shared_ptr<T>>;
  {
    t.V()
  }
  ->std::same_as<double>;
  {
    t.V(std::declval<double>())
  }
  ->std::same_as<std::shared_ptr<T>>;
  {
    t.E()
  }
  ->std::same_as<double>;
  {
    t.E(std::declval<double>())
  }
  ->std::same_as<std::shared_ptr<T>>;
};
}  // namespace Jessica::Data::Load

#endif

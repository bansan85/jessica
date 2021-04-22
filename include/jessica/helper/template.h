#pragma once

namespace Jessica::Helper
{
// Extract the type inside nested container.
// Example:
// ExtractRootType<std::vector<std::list<int>>>::Root will return int.
template <typename...>
struct ExtractRootType;

template <typename Args>
struct ExtractRootType<Args>
{
  using Root = Args;
};

template <template <typename...> class Container, typename Args>
struct ExtractRootType<Container<Args>>
{
  using Root = typename ExtractRootType<Args>::Root;
};

// Generate an assert in general template function
// to force specialization.
template <typename T>
struct StaticAssert : std::false_type
{
};
}  // namespace Jessica::Helper

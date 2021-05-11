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

// Container may have multiple type.
// Extract at the lowest level the nth type (first is 0).
// Example:
// ExtractRootTypeN<1, std::list<std::list<std::list<std::pair<int, double>>>>>::Root will return double.
template <std::size_t N, typename T, typename... types>
struct ExtractNthType
{
  using Root = typename ExtractNthType<N - 1, types...>::Root;
};

template <typename T, typename... types>
struct ExtractNthType<0, T, types...>
{
  using Root = T;
};

template <int N, typename...>
struct ExtractRootTypeN;

template <int N, template <typename...> class Container, typename... Args>
struct ExtractRootTypeN<N, Container<Args...>>
{
  using Root = typename ExtractNthType<N, Args...>::Root;
};

template <int N, template <typename...> class Container, typename Args>
struct ExtractRootTypeN<N, Container<Args>>
{
  using Root = typename ExtractRootTypeN<N, Args>::Root;
};

// Generate an assert in general template function
// to force specialization.
template <typename T>
struct StaticAssert : std::false_type
{
};
}  // namespace Jessica::Helper

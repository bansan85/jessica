#pragma once

namespace Jessica::Helper
{
// Extract the type inside nested container.
// Example:
// ExtractRootType<std::vector<std::list<int>>>::type will return int.
template <typename...>
struct ExtractRootType;

template <typename Args>
struct ExtractRootType<Args>
{
  using type = Args;
};

template <template <typename...> class Container, typename Args>
struct ExtractRootType<Container<Args>>
{
  using type = typename ExtractRootType<Args>::type;
};

// Container may have multiple type.
// Extract at the lowest level the nth type (first is 0).
// Example:
// ExtractRootTypeN<1, std::list<std::list<std::list<std::pair<int, double>>>>>::Root will return double.
template <std::size_t N, typename T, typename... types>
struct ExtractNthType
{
  using type = typename ExtractNthType<N - 1, types...>::type;
};

template <typename T, typename... types>
struct ExtractNthType<0, T, types...>
{
  using type = T;
};

template <int N, typename...>
struct ExtractRootTypeN;

template <int N, template <typename...> class Container, typename... Args>
struct ExtractRootTypeN<N, Container<Args...>>
{
  using type = typename ExtractNthType<N, Args...>::type;
};

template <int N, template <typename...> class Container, typename Args>
struct ExtractRootTypeN<N, Container<Args>>
{
  using type = typename ExtractRootTypeN<N, Args>::type;
};

// Generate an assert in general template function
// to force specialization.
template <typename T>
struct StaticAssert : std::false_type
{
};

// Lazy load of template.
// Usage: LazyLoadIdentity<U>::type
template <class T>
struct LazyLoadIdentity
{
  using type = T;
};

template <std::size_t N, typename... types>
struct LazyLoadExtractNthType
    : LazyLoadIdentity<
          typename Jessica::Helper::ExtractNthType<N, types...>::type>
{
};
}  // namespace Jessica::Helper

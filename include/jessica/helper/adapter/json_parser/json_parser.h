#pragma once

#include <concepts>
#include <string>
#include <vector>

namespace jessica
{
template <typename T>
concept JsonParserReader = requires(T t, const std::string& str,
                                    const std::vector<std::string>& vec)
{
  {
    t.ReadStr(str)
    } -> std::same_as<void>;
  {
    t.ReadFile(str)
    } -> std::same_as<void>;
  {
    t.Get(vec)
    } -> std::same_as<std::string>;
};

template <typename T, typename U>
concept JsonParserWriter = requires(T t, U u,
                                    const std::vector<std::string>& vec)
{
  {
    t.Set(vec, u)
    } -> std::same_as<T>;
};

template <typename T, typename U>
concept JsonParser = JsonParserReader<T> && JsonParserWriter<T, U>;
}  // namespace jessica

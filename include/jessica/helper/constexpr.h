#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <utility>

// Idea from https://xuhuisun.com/post/c++-weekly-2-constexpr-map/

namespace jessica
{

template <typename Key, typename Value, std::size_t Size>
class ConstExprMap
{
 public:
  constexpr explicit ConstExprMap(std::array<std::pair<Key, Value>, Size> data)
      : data_(std::move(data))
  {
  }

  [[nodiscard]] constexpr Value at(const Key& key) const
  {
    const auto itr =
        std::find_if(begin(data_), end(data_),
                     [&key](const auto& v) { return v.first == key; });
    if (itr == end(data_))
    {
      throw std::range_error("Not Found");
    }
    return itr->second;
  }

 private:
  std::array<std::pair<Key, Value>, Size> data_;
};

}  // namespace jessica

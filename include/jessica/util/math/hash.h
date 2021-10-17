#pragma once

#include <cstdint>

namespace jessica
{
constexpr uint64_t djb2(char const* str)
{
  uint64_t hash = 5381;
  uint64_t c = 0;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  while ((c = static_cast<unsigned char>(*str++)) != 0)
  {
    hash = hash * 33UL + c;
  }

  return hash;
}
}  // namespace jessica

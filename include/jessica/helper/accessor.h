#pragma once

#include <cstdint>

#include <jessica/util/math/hash.h>

namespace jessica
{
constexpr uint64_t operator""_f(const char* accessor, size_t /*size*/)
{
  return djb2(accessor);
}

}  // namespace jessica

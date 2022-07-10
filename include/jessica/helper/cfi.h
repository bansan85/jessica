#pragma once

#include <sstream>
#include <string>

namespace jessica
{
// std::to_string generates:
// /usr/lib/gcc/x86_64-pc-linux-gnu/11.1.0/include/g++-v11/ext/string_conversions.h:111:25: runtime error: control flow integrity check for type 'int (char *, unsigned long, const char *, __va_list_tag *)' failed during indirect function call
// (/root/prog/build/test/backend/data/test_load_vertical_eccentric+0x409d60): note: _init defined here
template <typename T>
std::string cfi_to_string(T number)
{
  std::ostringstream s;
  s << number;
  return s.str();
}

// std::stoul generates:
// /usr/lib/gcc/x86_64-pc-linux-gnu/11.2.1/include/g++-v11/ext/string_conversions.h:80:27: runtime error: control flow integrity check for type 'unsigned long (const char *, char **, int)' failed during indirect function call
template <typename T>
// cppcheck-suppress passedByValue
T cfi_to_number(const std::string_view text)
{
  std::stringstream s;
  T num;
  s << text;
  s >> num;
  return num;
}

}  // namespace jessica

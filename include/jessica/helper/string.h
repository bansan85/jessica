#pragma once

#include <string>

namespace jessica
{
class String
{
 public:
  template <typename Iter>
  static std::string join(Iter begin, Iter end, const std::string& separator)
  {
    std::ostringstream result;
    if (begin != end)
    {
      result << *begin++;
    }
    while (begin != end)
    {
      result << separator << *begin++;
    }
    return result.str();
  }
};
}  // namespace jessica

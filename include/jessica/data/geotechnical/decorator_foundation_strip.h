#pragma once

#include <cstddef>
#include <memory>
#include <utility>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/macro.h>
#include <jessica/util/decorator/start.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC DECORATOR_START_HEADER(FoundationStrip)
{
 public:
  DECORATOR_START_RULE_OF_FIVE(FoundationStrip);

  DECORATOR_CLONE(FoundationStrip)

  DECORATOR_START_GET(B)
  DECORATOR_START_SET(FoundationStrip, B)
};

template <typename T>
class JESSICA_DLL_PUBLIC DECORATOR_END_HEADER(FoundationStrip)
{
 public:
  using RootType = T;

  DECORATOR_END_RULE_OF_FIVE(FoundationStrip);

  DECORATOR_END_GET(B)
  DECORATOR_END_SET(B)
};
}  // namespace jessica

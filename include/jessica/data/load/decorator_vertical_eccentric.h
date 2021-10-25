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
class JESSICA_DLL_PUBLIC DECORATOR_START_HEADER(VerticalEccentric)
{
 public:
  DECORATOR_START_RULE_OF_FIVE(VerticalEccentric);

  DECORATOR_CLONE(VerticalEccentric)

  DECORATOR_START_GET(E)
  DECORATOR_START_SET(VerticalEccentric, E)

  DECORATOR_START_GET(V)
  DECORATOR_START_SET(VerticalEccentric, V)
};

template <typename T>
class JESSICA_DLL_PUBLIC DECORATOR_END_HEADER(VerticalEccentric)
{
 public:
  using RootType = T;

  DECORATOR_END_RULE_OF_FIVE(VerticalEccentric);

  DECORATOR_END_GET(E)
  DECORATOR_END_SET(E)

  DECORATOR_END_GET(V)
  DECORATOR_END_SET(V)
};
}  // namespace jessica

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
class DECORATOR_START_HEADER(Meyerhof)
{
 public:
  DECORATOR_START_RULE_OF_FIVE(Meyerhof);

  DECORATOR_CLONE(Meyerhof)

  DECORATOR_START_GET(B_)
  DECORATOR_START_GET(Qref)

  DECORATOR_START_GET(Foundation)
  DECORATOR_START_SET(Meyerhof, Foundation)

  DECORATOR_START_GET(Load)
  DECORATOR_START_SET(Meyerhof, Load)
};

template <typename T>
class DECORATOR_END_HEADER(Meyerhof)
{
 public:
  using RootType = T;

  DECORATOR_END_RULE_OF_FIVE(Meyerhof);

  DECORATOR_END_GET(B_)
  DECORATOR_END_GET(Qref)

  DECORATOR_END_GET(Foundation)
  DECORATOR_END_SET(Foundation)

  DECORATOR_END_GET(Load)
  DECORATOR_END_SET(Load)
};
}  // namespace jessica

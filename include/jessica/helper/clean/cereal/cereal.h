#pragma once

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
  #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
  #pragma clang diagnostic ignored "-Wdeprecated-this-capture"
#endif

#include <cereal/cereal.hpp>

#ifdef __clang__
  #pragma clang diagnostic pop
#endif

#pragma once

#if __clang__
  #define NO_DESTROY [[clang::no_destroy]]
#else
  #define NO_DESTROY
#endif

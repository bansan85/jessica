#pragma once

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wextra-semi"
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wcast-qual"
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
  #pragma clang diagnostic ignored "-Wcovered-switch-default"
  #pragma clang diagnostic ignored "-Wshadow-uncaptured-local"
  #pragma clang diagnostic ignored "-Wshadow"
  #pragma clang diagnostic ignored "-Wsign-conversion"
  #pragma clang diagnostic ignored "-Wsuggest-destructor-override"
  #pragma clang diagnostic ignored "-Wnewline-eof"
  #pragma clang diagnostic ignored "-Wformat-nonliteral"
  #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
  #pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-dtor"
  #pragma clang diagnostic ignored "-Wunused-parameter"
  #pragma clang diagnostic ignored "-Wimplicit-fallthrough"
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
  #pragma clang diagnostic ignored "-Wctad-maybe-unsupported"
  #pragma clang diagnostic ignored "-Wsuggest-override"
  #pragma clang diagnostic ignored "-Wdeprecated-copy-with-dtor"
#endif

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable : 4242)
  #pragma warning(disable : 4244)
  #pragma warning(disable : 4365)
  #pragma warning(disable : 4435)
  #pragma warning(disable : 4866)
#endif

#include <drogon/drogon.h>

#ifdef __clang__
  #pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

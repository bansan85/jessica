#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_JESSICA_DLL
    #ifdef __GNUC__
      #define JESSICA_DLL_PUBLIC __attribute__((dllexport))
    #else
      #define JESSICA_DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define JESSICA_DLL_PUBLIC __attribute__((dllimport))
    #else
      #define JESSICA_DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define JESSICA_DLL_LOCAL
#else
  #define JESSICA_DLL_PUBLIC __attribute__((visibility("default")))
  #define JESSICA_DLL_LOCAL __attribute__((visibility("hidden")))
#endif

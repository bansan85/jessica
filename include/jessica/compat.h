#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef JESSICA_BUILDING_DLL
    #ifdef __GNUC__
      #define JESSICA_DLL_PUBLIC __attribute__((dllexport))
    #else
      #define JESSICA_DLL_PUBLIC __declspec(dllexport)
    #endif
  #elif defined(JESSICA_STATIC_LIB)
    #define JESSICA_DLL_PUBLIC
  #else
    #ifdef __GNUC__
      #define JESSICA_DLL_PUBLIC __attribute__((dllimport))
    #else
      #define JESSICA_DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define JESSICA_DLL_LOCAL
#else
  #ifdef JESSICA_BUILDING_DLL
    #define JESSICA_DLL_PUBLIC __attribute__((visibility("default")))
  #elif defined(JESSICA_STATIC_LIB)
    #define JESSICA_DLL_PUBLIC __attribute__((visibility("hidden")))
  #else
    #define JESSICA_DLL_PUBLIC __attribute__((visibility("default")))
  #endif
  #define JESSICA_DLL_LOCAL __attribute__((visibility("hidden")))
#endif

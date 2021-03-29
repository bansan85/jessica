if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wno-global-constructors)
endif()

include(GoogleTest)
find_package(GTest)
if(${GTest_FOUND})
  message(STATUS "Found GTest.")
else(${GTest_FOUND})
  message(STATUS "Could not locate GTest. Try 3rdparty folder")
  set(GTEST_LIBRARY
      "${Jessica_SOURCE_DIR}/3rdparty/googletest/build/lib/Debug/gtestd.lib")
  set(GTEST_MAIN_LIBRARY
      "${Jessica_SOURCE_DIR}/3rdparty/googletest/build/lib/Debug/gtest_maind.lib"
  )
  set(GTEST_INCLUDE_DIR
      "${Jessica_SOURCE_DIR}/3rdparty/googletest/googletest/include")
  if(NOT EXISTS "${GTEST_LIBRARY}")
    message(FATAL_ERROR "${GTEST_LIBRARY} not found")
  endif()
  if(NOT EXISTS "${GTEST_MAIN_LIBRARY}")
    message(FATAL_ERROR "${GTEST_MAIN_LIBRARY} not found")
  endif()
  if(NOT EXISTS "${GTEST_INCLUDE_DIR}/gtest/gtest.h")
    message(FATAL_ERROR "${GTEST_MAIN_LIBRARY} not found")
  endif()
endif(${GTest_FOUND})

include_directories("${Jessica_SOURCE_DIR}/include" SYSTEM ${GTEST_INCLUDE_DIR})

add_subdirectory(data)
add_subdirectory(calc)

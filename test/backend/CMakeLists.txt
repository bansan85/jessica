if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wno-global-constructors)
endif()

include(GoogleTest)
find_package(GTest REQUIRED)
if(${GTest_FOUND})
  message(STATUS "Found GTest.")
  if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND (CMAKE_BUILD_TYPE STREQUAL
                                                  "Debug"))
    message(STATUS "Use GTest Debug.")
    set(GTEST_LIBRARY ${GTEST_LIBRARY_DEBUG})
    set(GTEST_MAIN_LIBRARY ${GTEST_MAIN_LIBRARY_DEBUG})
  endif()
endif(${GTest_FOUND})

include_directories("${Jessica_SOURCE_DIR}/include" SYSTEM ${GTEST_INCLUDE_DIR})

add_subdirectory(calc)
add_subdirectory(data)
add_subdirectory(util)

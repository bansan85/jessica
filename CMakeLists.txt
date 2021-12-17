cmake_minimum_required(VERSION 3.15)

project(Jessica VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR EMSCRIPTEN)
  set(USE_VCPKG ON)
endif()

include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)
include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

# Ubuntu/gcc don't have some xxx-config.cmake but vcpkg needs it.
if(USE_VCPKG)
  find_package(cereal REQUIRED)
  find_package(simdjson REQUIRED)
else()
  check_include_file_cxx("cereal/cereal.hpp" HAVE_CEREAL)
  if(NOT HAVE_CEREAL)
    message(FATAL_ERROR "Cereal library is needed")
  endif()

  if(DEFINED ENV{CI} AND CMAKE_CXX_FLAGS MATCHES "^.*-fanalyzer.*$")
    set(CMAKE_CXX_FLAGS_SAVE ${CMAKE_CXX_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-analyzer")
  endif()

  FetchContent_Declare(
    simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    GIT_SHALLOW TRUE
    GIT_TAG tags/v1.0.2)

  FetchContent_MakeAvailable(simdjson)

  if(DEFINED ENV{CI} AND CMAKE_CXX_FLAGS MATCHES "^.*-fanalyzer.*$")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_SAVE})
  endif()
endif()

option(BUILD_COVERAGE "Enable coverage")

# Set a default build type if none was specified
set(DEFAULT_BUILD_TYPE "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(DEFAULT_BUILD_TYPE "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
      "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "${DEFAULT_BUILD_TYPE}"
      CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(
    -Weverything
    -Werror
    -Wno-c++98-compat
    -Wno-padded
    -Wno-c++98-compat-pedantic
    -Wno-weak-vtables
    -Wno-c99-extensions
    -Wno-global-constructors
    -Wno-unneeded-member-function)
  check_cxx_compiler_flag("-Wno-return-std-move-in-c++11"
                          COMPILER_SUPPORTS_RETURN_STD_MOVE_IN_C_11)
  if(COMPILER_SUPPORTS_RETURN_STD_MOVE_IN_C_11)
    add_compile_options(-Wno-return-std-move-in-c++11)
  endif()
  check_cxx_compiler_flag("-Wno-reserved-identifier"
                          COMPILER_SUPPORTS_reserved_identifier)
  if(COMPILER_SUPPORTS_reserved_identifier)
    add_compile_options(-Wno-reserved-identifier)
  endif()
  if(BUILD_COVERAGE)
    add_compile_options(-O0 -fprofile-instr-generate -fcoverage-mapping -mllvm
                        -runtime-counter-relocation)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
  else(BUILD_COVERAGE)
    if(EMSCRIPTEN)
      add_compile_options("$<$<CONFIG:RELEASE>:-O2>"
                          "$<$<CONFIG:DEBUG>:-O0;-g>")
    else()
      add_compile_options("$<$<CONFIG:RELEASE>:-O3>"
                          "$<$<CONFIG:DEBUG>:-O0;-g>")
    endif()
  endif(BUILD_COVERAGE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Werror -Wextra -Wpedantic)
  if(BUILD_COVERAGE)
    add_compile_options(-O0 --coverage)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  else()
    add_compile_options("$<$<CONFIG:RELEASE>:-O3>" "$<$<CONFIG:DEBUG>:-O0;-g>")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(
    /Wall
    /WX
    /wd4251
    /wd4275
    /wd4514
    /wd4582
    /wd4623
    /wd4625
    /wd4626
    /wd4668
    /wd4710
    /wd4820
    /wd5026
    /wd5027
    /wd5039
    /wd5045
    /EHsc)
else()
  message(WARNING "${CMAKE_CXX_COMPILER_ID} is unknown")
endif()

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

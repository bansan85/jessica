cmake_minimum_required(VERSION 3.15)

project(Jessica VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckCXXCompilerFlag)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(spdlog REQUIRED)

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
    /wd4625
    /wd4626
    /wd4668
    /wd4710
    /wd4820
    /wd5026
    /wd5027
    /wd5039
    /wd5045)
else()
  message(WARNING "${CMAKE_CXX_COMPILER_ID} is unknown")
endif()

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

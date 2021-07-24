add_subdirectory(backend)

if(DEFINED EMSCRIPTEN)
  add_subdirectory(javascript)
  add_subdirectory(angular)
endif()

# add_subdirectory(main)

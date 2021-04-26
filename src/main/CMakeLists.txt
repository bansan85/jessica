if(EMSCRIPTEN)

else()
  add_executable(main1 ${CMAKE_CURRENT_SOURCE_DIR}/main1.cc)
  set_target_properties(main1 PROPERTIES LINK_FLAGS
                                         "-s DEMANGLE_SUPPORT=1 --bind")
  # target_link_libraries(main1 PUBLIC jessica)
endif()

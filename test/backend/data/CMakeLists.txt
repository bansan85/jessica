add_executable(test_geotechnical_foundation_strip
               ${CMAKE_CURRENT_SOURCE_DIR}/geotechnical/foundation_strip.cc)
add_executable(test_load_vertical_eccentric
               ${CMAKE_CURRENT_SOURCE_DIR}/load/vertical_eccentric.cc)

target_link_libraries(
  test_geotechnical_foundation_strip
  PRIVATE GTest::GTest GTest::Main nlohmann_json::nlohmann_json
          simdjson::simdjson spdlog::spdlog_header_only jessica)
target_link_libraries(
  test_load_vertical_eccentric
  PRIVATE GTest::GTest GTest::Main nlohmann_json::nlohmann_json
          simdjson::simdjson spdlog::spdlog_header_only jessica)

foreach(TESTI test_geotechnical_foundation_strip test_load_vertical_eccentric)
  add_test(NAME ${TESTI} COMMAND ${TESTI})
  if($<TARGET_RUNTIME_DLLS:${TESTI}>)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()

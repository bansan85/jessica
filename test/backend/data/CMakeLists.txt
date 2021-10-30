add_executable(test_geotechnical_foundation_strip
               ${CMAKE_CURRENT_SOURCE_DIR}/geotechnical/foundation_strip.cc)
add_executable(test_load_vertical_eccentric
               ${CMAKE_CURRENT_SOURCE_DIR}/load/vertical_eccentric.cc)

target_link_libraries(
  test_geotechnical_foundation_strip
  PRIVATE GTest::GTest GTest::Main spdlog::spdlog_header_only
          jessica::header_only)
target_link_libraries(
  test_load_vertical_eccentric
  PRIVATE GTest::GTest GTest::Main spdlog::spdlog_header_only
          jessica::header_only)
if(cereal_DIR)
  target_link_libraries(test_geotechnical_foundation_strip PRIVATE cereal)
  target_link_libraries(test_load_vertical_eccentric PRIVATE cereal)
endif()

add_test(NAME TestGeotechnicalFoundationStrip
         COMMAND test_geotechnical_foundation_strip)
add_test(NAME TestLoadVerticalEccentric COMMAND test_load_vertical_eccentric)

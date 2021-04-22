add_executable(test_geotechnical_foundation_strip
               ${CMAKE_CURRENT_SOURCE_DIR}/geotechnical/foundation_strip.cc)
target_link_libraries(test_geotechnical_foundation_strip PRIVATE GTest::GTest
                                                                 GTest::Main)
add_executable(test_load_vertical_eccentric
               ${CMAKE_CURRENT_SOURCE_DIR}/load/vertical_eccentric.cc)
target_link_libraries(test_load_vertical_eccentric PRIVATE GTest::GTest
                                                           GTest::Main)

add_test(NAME TestGeotechnicalFoundationStrip
         COMMAND test_geotechnical_foundation_strip)
add_test(NAME TestLoadVerticalEccentric COMMAND test_load_vertical_eccentric)

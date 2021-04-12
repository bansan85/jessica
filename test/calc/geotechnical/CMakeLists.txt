add_executable(test_calc_meyerhof ${CMAKE_CURRENT_SOURCE_DIR}/meyerhof.cc)
target_link_libraries(test_calc_meyerhof PRIVATE data calc_geotechnical
                                                 GTest::GTest GTest::Main)

add_test(NAME TestCalcMeyerhof COMMAND test_calc_meyerhof)

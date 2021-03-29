add_executable(test_calc_meyerhof ${CMAKE_CURRENT_SOURCE_DIR}/meyerhof.cc)
target_link_libraries(
  test_calc_meyerhof PRIVATE Threads::Threads data calc_geotechnical
                             ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

add_test(NAME TestCalcMeyerhof COMMAND test_calc_meyerhof)

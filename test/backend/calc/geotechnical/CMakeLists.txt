add_executable(test_calc_meyerhof ${CMAKE_CURRENT_SOURCE_DIR}/meyerhof.cc)

target_link_libraries(
  test_calc_meyerhof
  PRIVATE GTest::GTest GTest::Main nlohmann_json::nlohmann_json
          simdjson::simdjson spdlog::spdlog_header_only jessica::header_only)

add_test(NAME TestCalcMeyerhof COMMAND test_calc_meyerhof)

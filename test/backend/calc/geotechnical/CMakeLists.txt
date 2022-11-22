add_executable(test_calc_meyerhof ${CMAKE_CURRENT_SOURCE_DIR}/meyerhof.cc)

target_link_libraries(
  test_calc_meyerhof
  PRIVATE GTest::GTest GTest::Main nlohmann_json::nlohmann_json
          simdjson::simdjson spdlog::spdlog_header_only jessica)

foreach(TESTI test_calc_meyerhof)
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

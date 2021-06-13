add_executable(test_util_decorator ${CMAKE_CURRENT_SOURCE_DIR}/decorator.cc)
target_link_libraries(test_util_decorator PRIVATE GTest::GTest GTest::Main)

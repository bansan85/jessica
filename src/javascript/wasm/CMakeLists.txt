add_executable(jessica_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)
set_target_properties(
  jessica_js
  PROPERTIES LINK_FLAGS "-s EXPORT_NAME=jessica\
                         -s MODULARIZE=1\
                          --bind")
set_target_properties(jessica_js PROPERTIES OUTPUT_NAME "jessica")

add_executable(jessica_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)
set_target_properties(jessica_js PROPERTIES LINK_FLAGS "-s EXPORT_NAME=jessica \
-s MODULARIZE=1 --bind")
set_target_properties(jessica_js PROPERTIES OUTPUT_NAME "jessica")
target_link_libraries(jessica_js PRIVATE jessica spdlog::spdlog)

add_custom_command(
  TARGET jessica_js
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica.wasm"
          "${CMAKE_SOURCE_DIR}/src/angular/projects/app-main/src/assets"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica.js"
          "${CMAKE_SOURCE_DIR}/src/angular/projects/app-main/src/app/ui/main"
  COMMAND
    sed -i "s#jessica.wasm#assets/jessica.wasm#g"
    "${CMAKE_SOURCE_DIR}/src/angular/projects/app-main/src/app/ui/main/jessica.js"
)

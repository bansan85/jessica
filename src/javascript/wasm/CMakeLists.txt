add_executable(jessica_web_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)
set_target_properties(
  jessica_web_js PROPERTIES LINK_FLAGS "-s EXPORT_NAME=jessica \
-s MODULARIZE=1 --bind -s ENVIRONMENT=web,worker")
set_target_properties(jessica_web_js PROPERTIES OUTPUT_NAME "jessica-web")
target_link_libraries(jessica_web_js PRIVATE jessica spdlog::spdlog)

add_executable(jessica_node_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)
set_target_properties(
  jessica_node_js PROPERTIES LINK_FLAGS "-s EXPORT_NAME=jessica \
-s MODULARIZE=1 --bind")
set_target_properties(jessica_node_js PROPERTIES OUTPUT_NAME "jessica-node")
target_link_libraries(jessica_node_js PRIVATE jessica spdlog::spdlog)

add_custom_command(
  TARGET jessica_web_js
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.wasm"
    "${CMAKE_SOURCE_DIR}/src/angular/projects/lib-jessica/assets"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.js"
          "${CMAKE_SOURCE_DIR}/src/angular/projects/lib-jessica/src/lib"
  COMMAND
    sed -i "s#jessica-web.wasm#assets/jessica-web.wasm#g"
    "${CMAKE_SOURCE_DIR}/src/angular/projects/lib-jessica/src/lib/jessica-web.js"
)

set(EMSCRIPTEN_FLAGS "-s EXPORT_NAME=jessica -s MODULARIZE=1 --bind \
--no-entry -flto -s INVOKE_RUN=0 -s MALLOC='emmalloc'")

add_executable(jessica_web_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings-web.cpp)
target_link_libraries(
  jessica_web_js
  PRIVATE
    "$<$<CONFIG:RELEASE>:--closure 1 -s ASSERTIONS=0 -DNDEBUG>\
$<$<CONFIG:DEBUG>:--closure 0 -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 \
--emit-symbol-map -gsource-map --source-map-base \
http://localhost:4200/assets/> \
${EMSCRIPTEN_FLAGS} -s ENVIRONMENT=web,worker"
    jessica)
set_target_properties(jessica_web_js PROPERTIES OUTPUT_NAME "jessica-web")

add_executable(jessica_node_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings-test.cpp)
target_link_libraries(
  jessica_node_js
  PRIVATE
    "$<$<CONFIG:RELEASE>:--closure 1 -s ASSERTIONS=0 -DNDEBUG>\
$<$<CONFIG:DEBUG>:--closure 0 -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 \
--emit-symbol-map -gsource-map --source-map-base \
http://localhost:4200/assets/> \
${EMSCRIPTEN_FLAGS}"
    jessica
    spdlog::spdlog)
set_target_properties(jessica_node_js PROPERTIES OUTPUT_NAME "jessica-node")

add_custom_target(
  jessica_web_js2 ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "${CMAKE_SOURCE_DIR}/src/angular/projects/lib-jessica/src/assets"
  COMMAND sed -i "s#\\'jessica-web.wasm#\\'assets/jessica-web.wasm#g"
          "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.js"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.*"
          "${CMAKE_SOURCE_DIR}/src/angular/projects/lib-jessica/src/assets/")

add_dependencies(jessica_web_js2 jessica_web_js)

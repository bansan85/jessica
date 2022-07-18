set(EMSCRIPTEN_LDFLAGS
    "$<$<CONFIG:RELEASE>:--closure 1 -s ASSERTIONS=0>\
$<$<CONFIG:DEBUG>:--closure 0 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1 \
--emit-symbol-map -gsource-map --source-map-base \
http://localhost:4200/assets/ --profiling> -s EXPORT_NAME=jessica -s MODULARIZE=1 --bind \
-flto -s INVOKE_RUN=0 -s MALLOC='emmalloc'")

add_executable(jessica_web_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings-web.cc)
target_link_libraries(jessica_web_js PRIVATE jessica ${EMSCRIPTEN_LDFLAGS}
                                             "-s ENVIRONMENT=web,worker")
set_target_properties(jessica_web_js PROPERTIES OUTPUT_NAME "jessica-web")

add_executable(jessica_node_js ${CMAKE_CURRENT_SOURCE_DIR}/bindings-test.cc)
target_link_libraries(
  jessica_node_js PRIVATE jessica spdlog::spdlog ${EMSCRIPTEN_LDFLAGS}
                          "-s ENVIRONMENT=web,worker")
set_target_properties(jessica_node_js PROPERTIES OUTPUT_NAME "jessica-node")

add_custom_target(
  jessica_web_js2 ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "${CMAKE_SOURCE_DIR}/src/angular/projects/jessica/src/assets"
  COMMAND
    sed -i
    "s#[\\'\\\"]jessica-web.wasm[\\'\\\"]#\\'assets/jessica-web.wasm\\'#g"
    "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.js"
  COMMAND sed -i "s#var jessica#export var jessica#g"
          "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.js"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/jessica-web.*"
          "${CMAKE_SOURCE_DIR}/src/angular/projects/jessica/src/assets/")

add_dependencies(jessica_web_js2 jessica_web_js)

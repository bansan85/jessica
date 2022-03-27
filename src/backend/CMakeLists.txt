add_library(jessica STATIC dummy.cc)
if(NOT DEFINED EMSCRIPTEN)
  target_sources(jessica PRIVATE jessica/helper/adapter/webservice/restbed.cc)
endif()
target_include_directories(jessica PUBLIC ${Jessica_SOURCE_DIR}/include)
target_link_libraries(jessica PUBLIC ${RESTBED_LIB})

# Define to build Jessica as a library and not as a header only.
add_compile_definitions(JESSICA_BUILDING_DLL)

add_library(jessica_header_only INTERFACE)
add_library(jessica::header_only ALIAS jessica_header_only)
target_compile_definitions(jessica_header_only INTERFACE JESSICA_HEADER_ONLY)
target_link_libraries(jessica_header_only INTERFACE jessica)

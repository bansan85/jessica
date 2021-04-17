add_library(
  jessica STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/calc/geotechnical/meyerhof.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/data/geotechnical/foundation_strip.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/data/load/vertical_eccentric.cc)

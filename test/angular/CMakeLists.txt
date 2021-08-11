add_test(
  NAME NpmAngular
  COMMAND "npm" "run" "test" "--verbose"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/angular")

configure_file(jessica.test.js.in jessica.test.js)
configure_file(package.json.in package.json COPYONLY)

execute_process(COMMAND "echo" "Install npm dependencies for Jest")
execute_process(COMMAND "npm" "install" "jest" "--loglevel" "info"
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_test(
  NAME NpmTest
  COMMAND "npm" "run" "test" "--verbose"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

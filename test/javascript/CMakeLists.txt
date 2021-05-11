configure_file(jessica.test.js.in jessica.test.js)
configure_file(package.json.in package.json COPYONLY)

execute_process(COMMAND "npm" "install" "jest"
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/javascript/")

add_test(
  NAME NpmTest
  COMMAND "npm" "run" "test" "--verbose"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/javascript/")

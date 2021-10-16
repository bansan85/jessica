execute_process(COMMAND "echo" "Root npm dependencies for Angular")
execute_process(COMMAND "npm" "install" "--loglevel" "info"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
execute_process(COMMAND "echo" "Toolbox npm dependencies for Angular")
execute_process(
  COMMAND "npm" "install" "--loglevel" "info"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/projects/toolbox")
execute_process(COMMAND "echo" "Jessica npm dependencies for Angular")
execute_process(
  COMMAND "npm" "install" "--loglevel" "info"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/projects/jessica")
execute_process(COMMAND "echo" "App-main npm dependencies for Angular")
execute_process(
  COMMAND "npm" "install" "--loglevel" "info"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/projects/app-main")
execute_process(COMMAND "echo" "End of npm install")

add_custom_target(ng_app_main ALL)
add_custom_command(
  TARGET ng_app_main
  COMMAND "npm" "run" "preprocess"
  COMMAND "npm" "run" "build$<$<CONFIG:DEBUG>:-dev>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_dependencies(ng_app_main jessica_web_js2)

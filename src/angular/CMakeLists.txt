execute_process(COMMAND "echo" "Install npm dependencies for Angular")
execute_process(COMMAND "npm" "install" "--loglevel" "info"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_target(ng_app_main ALL)
add_custom_command(
  TARGET ng_app_main
  COMMAND "npm" "run" "build-app-main"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

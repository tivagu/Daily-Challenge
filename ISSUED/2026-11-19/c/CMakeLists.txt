cmake_minimum_required(VERSION 3.16)

project(crycare_router LANGUAGES C)

set (CMAKE_C_STANDARD 11)
add_executable(crycare_router src/main.c src/mqtt_router.c)

target_include_directories(crycare_router PRIVATE include)

set_target_properties(crycare_router PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
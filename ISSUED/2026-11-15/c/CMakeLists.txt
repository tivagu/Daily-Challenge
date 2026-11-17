cmake_minimum_required(VERSION 3.16)

project(crycare_fsm LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_executable(crycare_fsm src/alarm_fsm.c src/main.c)

target_link_libraries(crycare_fsm PRIVATE m) # ta có sử dụng lround cho compute f0_hz
target_include_directories(crycare_fsm PRIVATE include)
set_target_properties(crycare_fsm PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")




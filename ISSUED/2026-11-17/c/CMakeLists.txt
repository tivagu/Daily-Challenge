cmake_minimum_required(VERSION 3.16)

project(mqtt_introduce LANGUAGES C)

set (CMAKE_C_STANDARD 11)

add_executable(mqtt_introduce src/main.c src/mqtt_outbox.c)
target_include_directories(mqtt_introduce PRIVATE include)


set_target_properties(mqtt_introduce PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

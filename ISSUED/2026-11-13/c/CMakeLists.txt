cmake_minimum_required(VERSION 3.16)

project(crycare_pipeline LANGUAGES C)

add_executable(crycare_pipeline src/main.c src/audio_pipeline.c)

target_include_directories(crycare_pipeline PRIVATE include)

set_target_properties(crycare_pipeline PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(crycare_pipeline PRIVATE -Wall -Wextra)

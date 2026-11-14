cmake_minimum_required(VERSION 3.16)
project(static_pcm_ring LANGUAGES CXX)
add_executable(pcm_ring src/main.cpp src/audio_ring_buffer.cpp)
target_include_directories(pcm_ring PRIVATE include)
set_target_properties(pcm_ring PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set_target_properties(pcm_ring PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()
if(MSVC)
    target_compile_options(pcm_ring PRIVATE /W4)
else()
    target_compile_options(pcm_ring PRIVATE -Wall -Wextra -Wpedantic)
endif()

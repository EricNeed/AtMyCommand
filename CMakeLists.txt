cmake_minimum_required(VERSION 3.10.0)
project("AtMyCommand" VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0111 NEW)

file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")
add_executable(AtMyCommand ${CMAKE_SOURCE_DIR}\\src\\main.cpp ${SERVER_SOURCES})

include(${CMAKE_SOURCE_DIR}/cmake/get_dependancies.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/functions.cmake)

find_package(SDL3 REQUIRED PATHS ${SDL3_DIR})
find_package(SDL3_image REQUIRED PATHS ${SDL_Image_DIR})
find_package(SDL3_shadercross REQUIRED PATHS ${SDL3_shadercross_DIR})
find_package(spirv_cross_c_shared REQUIRED PATHS ${spirv_cross_c_shared_DIR})
target_link_libraries(AtMyCommand
    PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image
    SDL3_shadercross::SDL3_shadercross
)

target_include_directories(AtMyCommand PRIVATE ${CMAKE_SOURCE_DIR})

#shader compile
compileShader("${CMAKE_SOURCE_DIR}/src/shader/shader.hlsl" "spv" "DXIL" "vertex" "mainVSH")
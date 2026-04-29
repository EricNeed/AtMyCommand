cmake_minimum_required(VERSION 3.10.0)
project("AtMyCommand" VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0111 NEW)

file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")
add_executable(AtMyCommand ${CMAKE_SOURCE_DIR}\\src\\main.cpp ${SERVER_SOURCES})

include(${CMAKE_SOURCE_DIR}/cmake/get_dependancies.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/functions.cmake)

find_package(SDL3 REQUIRED PATHS ${SDL3_DIR})
find_package(SDL3_image REQUIRED PATHS ${SDL_Image_DIR})

target_link_libraries(AtMyCommand
    PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image
)

target_include_directories(AtMyCommand PRIVATE ${CMAKE_SOURCE_DIR})

#set output shader format here
set(Shader_Format "dxil")

#configure the setting inside of program
if(${Shader_Format} STREQUAL "dxil") 
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_DXIL)
elseif(${Shader_Format} STREQUAL "spv")
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_SPIRV)
elseif(${Shader_Format} STREQUAL "metal")
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_MSL)
endif()

#shader compile
compileShader("${CMAKE_SOURCE_DIR}/src/shader/shader.hlsl" ${Shader_Format} "vertex" "mainVSH")
compileShader("${CMAKE_SOURCE_DIR}/src/shader/shader.hlsl" ${Shader_Format} "fragment" "mainFSH") 
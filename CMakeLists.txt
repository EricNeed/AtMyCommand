cmake_minimum_required(VERSION 3.10.0)
project("AtMyCommand" VERSION 0.1.0 LANGUAGES C CXX)

add_executable(AtMyCommand ${CMAKE_SOURCE_DIR}\\src\\main.cpp)



#load the local config, if have one
if(EXISTS ${CMAKE_SOURCE_DIR}/local_config.cmake)
    message("local config file found")
    include(${CMAKE_SOURCE_DIR}/local_config.cmake)
endif()



#change these to your local library location
if(NOT DEFINED SDL3_DIR)
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\SDL3-devel-3.4.0-VC\\SDL3-3.4.0\\cmake") 
endif()
if(NOT DEFINED SDL_Image_DIR) 
    set(SDL_Image_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\SDL3_image-devel-3.4.0-VC\\SDL3_image-3.4.0\\cmake") 
endif()
if(NOT DEFINED SDL3_shadercross_DIR)
    set(SDL3_shadercross_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\libraries\\shadercross_built\\lib\\cmake\\SDL3_shadercross")
    set(spirv_cross_c_shared_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\SPIRV_Cross_built\\share\\spirv_cross_c_shared\\cmake")
endif()



#no need for configure(inside of project)
set(DirectXShaderCompiler_dxcompiler_BINARY ${CMAKE_SOURCE_DIR}/dependancies/dxcompiler.dll)
set(DirectXShaderCompiler_dxcompiler_LIBRARY ${CMAKE_SOURCE_DIR}/dependancies/dxcompiler.lib)
set(DirectXShaderCompiler_dxil_BINARY ${CMAKE_SOURCE_DIR}/dependancies/dxil.dll)


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

target_sources(AtMyCommand PRIVATE
    src/client/rendering.cpp
)
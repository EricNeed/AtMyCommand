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
if(NOT DEFINED SDL3_shadercross_DIR)#built version of these libs are aviliable in the github release
    set(SDL3_shadercross_EXE_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\libraries\\shadercross_built\\bin\\shadercross.exe")
    set(SDL3_shadercross_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\libraries\\shadercross_built\\lib\\cmake\\SDL3_shadercross")
    set(spirv_cross_c_shared_DIR "${CMAKE_SOURCE_DIR}\\dependancies\\libraries\\SPIRV_Cross_built\\share\\spirv_cross_c_shared\\cmake")
    set(DirectXShaderCompiler_INCLUDE_PATH "${CMAKE_SOURCE_DIR}\\dependancies\\libraries\\DirectXCompiler\\inc")
endif()

#no need for configure(inside of project)
set(DirectXShaderCompiler_dxcompiler_BINARY ${CMAKE_SOURCE_DIR}/dependancies/dxcompiler.dll)
set(DirectXShaderCompiler_dxcompiler_LIBRARY ${CMAKE_SOURCE_DIR}/dependancies/dxcompiler.lib)
set(DirectXShaderCompiler_dxil_BINARY ${CMAKE_SOURCE_DIR}/dependancies/dxil.dll)
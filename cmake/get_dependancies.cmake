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
if(NOT DEFINED Shadercross_DIR)
    set(Shadercross_DIR "${CMAKE_SOURCE_DIR}\\denpendanices\\shadercross_bundle\\bin\\shadercross.exe")
endif()
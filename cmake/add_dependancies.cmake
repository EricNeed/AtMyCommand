set(SDL3_DIR "D:\\Code\\C++\\Library\\SDL3-devel-3.4.12-mingw\\SDL3-3.4.12\\cmake")
set(SDL_Image_DIR "D:\\Code\\C++\\Library\\SDL3_image-devel-3.4.4-mingw\\SDL3_image-3.4.4\\cmake")
set(Shadercross_DIR "D:\\Code\\C++\\Library\\shader_cross_libs\\shadercross_bundle\\bin\\shadercross.exe")

find_package(SDL3 REQUIRED PATHS ${SDL3_DIR})
find_package(SDL3_image REQUIRED PATHS ${SDL_Image_DIR})

target_link_libraries(AtMyCommand
    PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image
)
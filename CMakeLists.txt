cmake_minimum_required(VERSION 3.28.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(AtMyCommand VERSION 0.0.1 LANGUAGES C CXX)
add_executable(AtMyCommand)

#sources
include(cmake/add_sources.cmake)

#dependancies
include(cmake/add_dependancies.cmake)


#shader compilation
set(Shader_Format "dxil")
include(cmake/compile_shader.cmake)



# {
#             "name": "Clang 20.1.8 x86_64-w64-windows-gnu",
#             "displayName": "Clang 20.1.8 x86_64-w64-windows-gnu",
#             "description": "Using compilers: C = d:\\Program Files\\Compilers\\MinGW\\clang64\\bin\\clang.exe, CXX = d:\\Program Files\\Compilers\\MinGW\\clang64\\bin\\clang++.exe",
#             "binaryDir": "${sourceDir}/out/build",
#             "cacheVariables": {
#                 "CMAKE_INSTALL_PREFIX": "${sourceDir}/out/install/${presetName}",
#                 "CMAKE_C_COMPILER": "d:/Program Files/Compilers/MinGW/clang64/bin/clang.exe",
#                 "CMAKE_CXX_COMPILER": "d:/Program Files/Compilers/MinGW/clang64/bin/clang++.exe",
#                 "CMAKE_BUILD_TYPE": "Debug"
#             },
#             "generator": "Ninja"
#         },


#            "compileCommands": "${workspaceFolder}\\out\\build\\gcc\\compile_commands.json",
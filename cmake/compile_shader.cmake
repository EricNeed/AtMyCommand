#command for compile shader
function(compileShader SHADER_SOURCE OUTPUT_EXTENSION RENDER_STAGE ENTRY_FUNCTION)
    #the name of the file
    get_filename_component(SOURCE_NAME ${SHADER_SOURCE} NAME_WE)
    #get the short name of the render stage as part of the output file name
    string(SUBSTRING ${RENDER_STAGE} 0 4 STAGE_SHORT)
    #the output file directory
    set(OUTPUT_FILE_DIR "${CMAKE_BINARY_DIR}\\shaders\\${SOURCE_NAME}.${STAGE_SHORT}.${OUTPUT_EXTENSION}")
    message("output dir is: ${OUTPUT_FILE_DIR}")

    #the command line function
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_DIR}
        COMMAND ${Shadercross_DIR} ${SHADER_SOURCE}
        -o ${OUTPUT_FILE_DIR}
        -t ${RENDER_STAGE}
        -e ${ENTRY_FUNCTION}
    )

    target_sources(AtMyCommand PRIVATE ${OUTPUT_FILE_DIR})
endfunction()



#configure the setting inside of program
if(${Shader_Format} STREQUAL "dxil")
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_DXIL)
elseif(${Shader_Format} STREQUAL "spv")
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_SPIRV)
elseif(${Shader_Format} STREQUAL "metal")
target_compile_definitions(AtMyCommand PRIVATE CrossShaderFormat=SDL_GPU_SHADERFORMAT_MSL)
endif()

#shader compile
compileShader("${CMAKE_SOURCE_DIR}/shaders/shader.hlsl" ${Shader_Format} "vertex" "mainVSH")
compileShader("${CMAKE_SOURCE_DIR}/shaders/shader.hlsl" ${Shader_Format} "fragment" "mainFSH") 
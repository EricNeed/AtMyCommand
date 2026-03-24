#command for compile shader
function(compileShader SHADER_SOURCE OUTPUT_EXTENSION OUTPUT_FORMAT RENDER_STAGE INTRY_FUNCTION)
    get_filename_component(SOURCE_NAME ${SHADER_SOURCE} NAME_WE)
    set(OUTPUT_FILE_DIR "${CMAKE_SOURCE_DIR}/build/shader/${SOURCE_NAME}.${OUTPUT_EXTENSION}")
    add_custom_command(
        OUTPUT "shader.spv"
        COMMAND ${SDL3_shadercross_EXE_DIR} ${SHADER_SOURCE}
        -o ${OUTPUT_FILE_DIR}
        -t ${RENDER_STAGE}
        -e ${INTRY_FUNCTION}
    )
    target_sources(AtMyCommand PRIVATE ${OUTPUT_FILE_DIR})
endfunction()














#command for compile shader
# function(compileShader SHADER_SOURCE OUTPUT_EXTENSION OUTPUT_FORMAT RENDER_STAGE INTRY_FUNCTION)
#     get_filename_component(SOURCE_NAME ${SHADER_SOURCE} NAME_WE)
#     set(OUTPUT_FILE_DIR "${CMAKE_SOURCE_DIR}/build/shader/${SOURCE_NAME}.${OUTPUT_EXTENSION}")
#     add_custom_command(
#         OUTPUT ${OUTPUT_FILE_DIR}
#         COMMAND ${SDL3_shadercross_EXE_DIR} ${SHADER_SOURCE}
#         -o ${OUTPUT_FILE_DIR}
#         -t ${RENDER_STAGE}
#         -e ${INTRY_FUNCTION}
#     )
#     target_sources(AtMyCommand PRIVATE ${OUTPUT_FILE_DIR})
# endfunction()
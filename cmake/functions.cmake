#command for compile shader
function(compileShader SHADER_SOURCE OUTPUT_EXTENSION RENDER_STAGE ENTRY_FUNCTION)
    #the name of the file
    get_filename_component(SOURCE_NAME ${SHADER_SOURCE} NAME_WE)
    #get the short name of the render stage as part of the output file name
    string(SUBSTRING ${RENDER_STAGE} 0 4 STAGE_SHORT)
    #the output file directory
    set(OUTPUT_FILE_DIR "${CMAKE_BINARY_DIR}\\shader\\${SOURCE_NAME}.${STAGE_SHORT}.${OUTPUT_EXTENSION}")

    #the command line function
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_DIR}
        COMMAND "D:\\Code\\C++\\Library\\shader_cross_libs\\shadercross_bundle\\bin\\shadercross.exe" ${SHADER_SOURCE}
        -o ${OUTPUT_FILE_DIR}
        -t ${RENDER_STAGE}
        -e ${ENTRY_FUNCTION}
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
#command for compile shader
function(compileShader SHADER_SOURCE OUTPUT_EXTENSION OUTPUT_FORMAT RENDER_STAGE INTRY_FUNCTION)
    get_filename_component(SOURCE_NAME ${SHADER_SOURCE} NAME_WE)
    set(OUTPUT_FILE_DIR "${CMAKE_SOURCE_DIR}/build/shader/${SOURCE_NAME}.${OUTPUT_EXTENSION}")
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_DIR}
        COMMAND shadercross ${SHADER_SOURCE}
        -o ${OUTPUT_FILE_DIR}
        -t ${RENDER_STAGE}
        -e ${INTRY_FUNCTION}
        -s HLSL
        -d ${OUTPUT_FORMAT}
    )
    target_sources(AtMyCommand PRIVATE ${OUTPUT_FILE_DIR})
endfunction()
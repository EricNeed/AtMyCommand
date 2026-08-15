target_sources(AtMyCommand
    PRIVATE
        src/main.cpp
)

#module declearation
target_sources(AtMyCommand
    PUBLIC
        FILE_SET CXX_MODULES BASE_DIRS src
        FILES
            src/client/rendering/gpu/create_pipeline.cppm
            src/client/rendering/gpu/pipeline_a.cppm
            src/client/rendering/render_game.cppm
            src/client/tick_client.cppm
            src/client/rendering/gpu/helper_functions/test_shapes.cppm
            src/client/rendering/gpu/helper_functions/upload_textures.cppm

)
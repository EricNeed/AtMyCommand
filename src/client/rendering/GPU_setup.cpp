#include<src/client/rendering/GPU_setup.h>

const int hi = 20;

SDLGPUSetup::SDLGPUSetup(){
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[SDLGPUSetup::SDLGPUSetup]: Creating window");
    sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, NULL, NULL);
    SDL_ClaimWindowForGPUDevice(gpu_device, sdl_window);
    int a = hi;
}

bool SDLGPUSetup::setupShaders(SDL_GPUShader* vert_shader, SDL_GPUShader* frag_shader){
    //vertex shader:
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading game shaders");
    size_t vsh_size;
    Uint8 *vsh_code = (Uint8 *)SDL_LoadFile("shader/shader.vert.dxil", &vsh_size);
    SDL_GPUShaderCreateInfo vsh_info = {
        .code_size = vsh_size,
        .code = vsh_code,
        .entrypoint = "mainVSH",              // entry‑point name
        .format = SDL_GPU_SHADERFORMAT_DXIL,  // DXIL on D3D12
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,  // shader stage
        .num_samplers = 0,                    // number of SamplerState in your shader
        .num_storage_textures = 0,           // UAVs bound as textures
        .num_storage_buffers = 0,            // UAVs bound as buffers
        .num_uniform_buffers = 0,            // cbuffer / uniform buffer slots used
        .props = 0                           // 0 if no extensions
    };
    vert_shader = SDL_CreateGPUShader(gpu_device, &vsh_info);

    //fragment shader:
    size_t fsh_size;
    Uint8 *fsh_code = (Uint8 *)SDL_LoadFile("Shader/shader.frag.dxil", &fsh_size);
    SDL_GPUShaderCreateInfo fsh_info = {
        .code_size = fsh_size,
        .code = fsh_code,
        .entrypoint = "mainFSH",              // entry‑point name
        .format = SDL_GPU_SHADERFORMAT_DXIL,  // DXIL on D3D12
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,  // shader stage
        .num_samplers = 0,                    // number of SamplerState in your shader
        .num_storage_textures = 0,           // UAVs bound as textures
        .num_storage_buffers = 0,            // UAVs bound as buffers
        .num_uniform_buffers = 0,            // cbuffer / uniform buffer slots used
        .props = 0                           // 0 if no extensions
    };
    frag_shader = SDL_CreateGPUShader(gpu_device, &fsh_info);

    if(vert_shader == nullptr || frag_shader == nullptr){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shader creation failed, %s", SDL_GetError());
        return false;
    }

    return true;
}


bool SDLGPUSetup::setupPipeline(){
    SDL_GPUShader *vert_shader;
    SDL_GPUShader *frag_shader;
    setupShaders(vert_shader, frag_shader);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating game GPU pipeline");
    //vertex buffer discriptions
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Registering vertex shader information");
    SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1]{{
        .slot = 0,
        .pitch = sizeof(SDL_Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    }};
    //vertex attribute
    SDL_GPUVertexAttribute vertexAttributes[2]{
        {//position x, y, depth
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = 0,
        },
        {//color r, g, b, a
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            .offset = sizeof(float) * 3,
        }
    };
    //color target discription
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Registering pipeline color target discription");
    SDL_GPUColorTargetDescription colorTargetDescriptions[1]{{
        .format = SDL_GetGPUSwapchainTextureFormat(sdl_gpu_device, sdl_window),
        .blend_state = {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .enable_blend = true,
        },
    }};
    //pipeline info
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Registering pipeline information");
    SDL_GPUGraphicsPipelineCreateInfo pipeline_info{
        .vertex_shader = vsh_shader,
        .fragment_shader = fsh_shader,
        .vertex_input_state = {
            .vertex_buffer_descriptions = vertexBufferDesctiptions,
            .num_vertex_buffers = 1,
            .vertex_attributes = vertexAttributes,
            .num_vertex_attributes = 2,
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {
            .color_target_descriptions = colorTargetDescriptions,
            .num_color_targets = 1,
        },
    };
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Creating pipeline");
    SDL_GPUGraphicsPipeline* graphics_pipeline = SDL_CreateGPUGraphicsPipeline(sdl_gpu_device, &pipeline_info);
    if(graphics_pipeline == nullptr){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GPU pipeline creation failed, %s", SDL_GetError());
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Release GPU shaders");
    SDL_ReleaseGPUShader(sdl_gpu_device, vsh_shader);//dont need shader no more after create pipeline
    SDL_ReleaseGPUShader(sdl_gpu_device, fsh_shader);
}
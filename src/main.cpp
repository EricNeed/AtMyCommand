#include<SDL3/SDL.h>
#include"src/client/client_main.h"

SDL_Window* sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE);
SDL_GPUDevice* sdl_gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, NULL, NULL);

struct Vertex{
    float x, y, z, r, g, b, s;
};



bool game_running = true;
SDL_Event sdl_event;
void processSDLEvents(){
    //process all current SDL events
    while(SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type){
            case SDL_EVENT_QUIT: game_running = false;
        default: break;
        }
    }
}

SDL_GPUGraphicsPipelineCreateInfo pipeline_info = {};

int main(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game is Running");
    SDL_ClaimWindowForGPUDevice(sdl_gpu_device, sdl_window);


    SDL_GPUColorTargetDescription color_target_desc = {
        .format = SDL_GetGPUSwapchainTextureFormat(sdl_gpu_device, sdl_window)
    };



    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Prepare vertex shader file");

    size_t vsh_size;
    Uint8 *vsh_code = (Uint8 *)SDL_LoadFile("shader/shader.vert.dxil", &vsh_size);

    SDL_GPUShaderCreateInfo vsh_info = {
        .code_size = vsh_size,
        .code = vsh_code,
        .entrypoint = "mainVSH",              // your entry‑point name
        .format = SDL_GPU_SHADERFORMAT_DXIL,  // DXIL on D3D12
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,  // or SDL_GPU_SHADERSTAGE_FRAGMENT, etc.
        .num_samplers = 0,                    // number of SamplerState in your shader
        .num_storage_textures = 0,           // UAVs bound as textures
        .num_storage_buffers = 0,            // UAVs bound as buffers
        .num_uniform_buffers = 0,            // cbuffer / uniform buffer slots used
        .props = 0                           // 0 if no extensions
    };

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Prepare fragment shader file");

    size_t fsh_size;
    Uint8 *fsh_code = (Uint8 *)SDL_LoadFile("Shader/shader.frag.dxil", &fsh_size);

    SDL_GPUShaderCreateInfo fsh_info = {
        .code_size = fsh_size,
        .code = fsh_code,
        .entrypoint = "mainFSH",              // your entry‑point name
        .format = SDL_GPU_SHADERFORMAT_DXIL,  // DXIL on D3D12
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,  // or SDL_GPU_SHADERSTAGE_FRAGMENT, etc.
        .num_samplers = 0,                    // number of SamplerState in your shader
        .num_storage_textures = 0,           // UAVs bound as textures
        .num_storage_buffers = 0,            // UAVs bound as buffers
        .num_uniform_buffers = 0,            // cbuffer / uniform buffer slots used
        .props = 0                           // 0 if no extensions
    };


    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Assembling gpu create info");

    SDL_GPUGraphicsPipelineCreateInfo;

    SDL_GPUVertexBufferDescription vert_buffer_disc{0, 12, SDL_GPU_VERTEXINPUTRATE_VERTEX};

    SDL_GPUVertexAttribute vertex_attrs[3] = {{
        // Attribute 0: X coordinate (TEXCOORD0 in shader)
        .location = 0,           // Matches shader: layout(location=0) in float x;
        .buffer_slot = 0,        // From vertex buffer slot 0
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, // Single float (4 bytes)
        .offset = 0              // Starts at byte 0 in vertex
    },
    {
        // Attribute 1: Y coordinate (TEXCOORD1 in shader)  
        .location = 1,           // layout(location=1) in float y;
        .buffer_slot = 0,        // Same buffer slot 0
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, // Single float
        .offset = 4              // After X (byte 4)
    },
    {
        // Attribute 2: Z coordinate (TEXCOORD2 in shader)
        .location = 2,           // layout(location=2) in float z;
        .buffer_slot = 0,        // Same buffer slot 0  
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, // Single float
        .offset = 8              // After X+Y (byte 8)
    }};

    SDL_GPUVertexInputState pipeline_input_state{
        .vertex_buffer_descriptions = &vert_buffer_disc,
        .num_vertex_buffers = 1,
        .vertex_attributes = vertex_attrs,
        .num_vertex_attributes = 3,
    };

    pipeline_info = {
        .vertex_shader = SDL_CreateGPUShader(sdl_gpu_device, &vsh_info),
        .fragment_shader = SDL_CreateGPUShader(sdl_gpu_device, &fsh_info),
        .vertex_input_state = pipeline_input_state,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {&color_target_desc, 1, SDL_GPU_TEXTUREFORMAT_D16_UNORM, false}
    };
    
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating gpu pipeline");
    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(sdl_gpu_device, &pipeline_info);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Prepare drawing verticies (constant for now)");
    const float triangle_vertices[9] = {
        -0.5f, -0.5f, 0.0f,  // Bottom-left
        0.5f, -0.5f, 0.0f,  // Bottom-right
        0.0f,  0.5f, 0.0f   // Top
    };
    
    
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "create gpu buffer");
    SDL_GPUBufferCreateInfo triangle_vert_info{
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = 36,
        .props = 0,
    };
    
    SDL_GPUBuffer* verticie_buffer = SDL_CreateGPUBuffer(sdl_gpu_device, &triangle_vert_info);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "create transfer buffer");
    SDL_GPUTransferBufferCreateInfo transfer_info{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = 36,
        .props = 0,
    };

    SDL_GPUTransferBuffer* transfer_bufer = SDL_CreateGPUTransferBuffer(sdl_gpu_device, &transfer_info);
    SDL_GPUTransferBufferLocation transfer_location{
        .transfer_buffer = transfer_bufer,
        .offset = 0,
    };

    SDL_GPUBufferRegion buffer_reigion{
        .buffer = verticie_buffer,
        .offset = 0,
        .size = 36,
    };

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "upload transfer buffer");
    SDL_GPUCommandBuffer* upload_cmd = SDL_AcquireGPUCommandBuffer(sdl_gpu_device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
    SDL_UploadToGPUBuffer(copy_pass, &transfer_location, 0, triangle_vertices);
    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(upload_cmd);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game loop begin");

    while(game_running){
        
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(sdl_gpu_device);
        SDL_GPUTexture* swapchainTex;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmd, sdl_window, &swapchainTex, NULL, NULL);
        if (swapchainTex) {
            SDL_GPUColorTargetInfo colorTarget = { .texture = swapchainTex, .clear_color = {0, 0, 0, 1}, .load_op = SDL_GPU_LOADOP_CLEAR, .store_op = SDL_GPU_STOREOP_STORE };
            SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmd, &colorTarget, 1, NULL);
            
            SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
            
            SDL_GPUBufferBinding buffer_bind{
                .buffer = verticie_buffer, 
                .offset = 0 
            };

            SDL_BindGPUVertexBuffers(renderPass, 0, &buffer_bind, 1);

            // Draw 3 vertices (1 triangle)
            SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
            
            SDL_EndGPURenderPass(renderPass);
        }
        SDL_SubmitGPUCommandBuffer(cmd);


        processSDLEvents();
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clean up instances");
    SDL_ReleaseGPUGraphicsPipeline(sdl_gpu_device, pipeline);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}
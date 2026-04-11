#include<SDL3/SDL.h>
#include"src/client/client_main.h"

SDL_Window* sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
SDL_GPUDevice* sdl_gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, NULL, NULL);


// the vertex input layout, (0,0):center, (-1,-1)bottom left, (1, 1)top right
struct Vertex
{
    float x, y, z;      //vec3 position
    float r, g, b, a;   //vec4 color
};
// a list of vertices
static Vertex vertices[]
{
    {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},     // top vertex
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}     // bottom right vertex
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


int main(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game is Running");
    SDL_ClaimWindowForGPUDevice(sdl_gpu_device, sdl_window);


    //SHADER!!!!!!!:(
    //vertex shader:
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading game shaders");
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
    SDL_GPUShader* vsh_shader = SDL_CreateGPUShader(sdl_gpu_device, &vsh_info);

    //fragment shader:
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
    SDL_GPUShader* fsh_shader = SDL_CreateGPUShader(sdl_gpu_device, &fsh_info);
    if(vsh_shader == nullptr || fsh_shader == nullptr){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shader creation failed, %s", SDL_GetError());
    }


    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating game GPU pipeline");
    //vertex buffer discriptions
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "  - Registering vertex shader information");
    SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1]{{
        .slot = 0,
        .pitch = sizeof(Vertex),
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

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating GPU buffer and trasfer buffer");
    //color target
    SDL_GPUColorTargetInfo gpu_color_target_info = {
        .clear_color = {1.0, 1.0, 1.0, 1.0},//what color to clear the screen to
        .load_op = SDL_GPU_LOADOP_CLEAR,// or "SDL_GPU_LOADOP_LOAD" to keep the previous content
        .store_op = SDL_GPU_STOREOP_STORE,// store the content to the texture
    };

    //create a buffer on the gpu
    SDL_GPUBufferCreateInfo vertex_buffer_info{
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(vertices),//size of the buffer
        .props = 0,// propertie ID if need extension
    };
    SDL_GPUBuffer* vertex_buffer = SDL_CreateGPUBuffer(sdl_gpu_device, &vertex_buffer_info);//very expensive to create, reuse it

    //create a transfer buffer
    SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, //this buffer transfer data to the gpu
        .size = (uint32_t)size_t(vertices),
        .props = 0,
    };
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(sdl_gpu_device, &transfer_buffer_info);
    //the passing info about the transfer buffer
    SDL_GPUTransferBufferLocation transfer_location{
        .transfer_buffer = transfer_buffer,
        .offset = 0,
    };

    //info about the destination of the copy pass, which is the buffer we created
    SDL_GPUBufferRegion copy_destination{
        .buffer = vertex_buffer,
        .offset = 0,// begin writing from the first vertex
        .size = sizeof(vertices),
    };


    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game loop begin");

    while(game_running){
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(sdl_gpu_device);
        

        //fill in the transfer buffer with verticies:
        Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(sdl_gpu_device, transfer_buffer, false);
        data[0] = vertices[0];
        data[1] = vertices[1];
        data[2] = vertices[2];
        // SDL_memcpy(data, vertices, sizeof(vertices));//or do this, this copy the entire verticies array to the transfer
        SDL_UnmapGPUTransferBuffer(sdl_gpu_device, transfer_buffer);//clean up

        //upload to copy pass
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
        SDL_UploadToGPUBuffer(copyPass, &transfer_location, &copy_destination, true);
        SDL_EndGPUCopyPass(copyPass);

        
        //prepare the render pass
        SDL_GPUTexture* swapchainTexture;
        Uint32 width, height;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmd, sdl_window, &swapchainTexture, &width, &height);
        if(swapchainTexture == NULL){continue;}//can sometime false, for example: window minimized
        gpu_color_target_info.texture = swapchainTexture;// window's swapchain texture

        //pass info about drawing stuff onto screen
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(cmd, &gpu_color_target_info, 1, NULL);
        SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);//bind the
        SDL_GPUBufferBinding bufferBindings[1]{{
            .buffer = vertex_buffer,
            .offset = 0,
        }};
        SDL_BindGPUVertexBuffers(render_pass, 0, bufferBindings, 1);
        SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
        

        //clean up after every draw
        SDL_EndGPURenderPass(render_pass);
        SDL_SubmitGPUCommandBuffer(cmd);
        processSDLEvents();
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clean up instances");
    SDL_ReleaseGPUGraphicsPipeline(sdl_gpu_device, graphics_pipeline);
    SDL_ReleaseGPUBuffer(sdl_gpu_device, vertex_buffer);
    SDL_ReleaseGPUTransferBuffer(sdl_gpu_device, transfer_buffer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}
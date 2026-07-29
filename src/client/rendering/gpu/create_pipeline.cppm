module;
#include <SDL3/SDL.h>
export module Create_New_Pipeline;


import Pipeline_A;


export SDL_Window* sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
export SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, NULL);


// the vertex input layout, (0,0):center, (-1,-1)bottom left, (1, 1)top right
struct Vertex
{
    float x, y, z;      //vec3 position
    float r, g, b, a;   //vec4 color
};
// a list of vertices
Vertex vertices[]
{
    {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},     // top vertex
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}     // bottom right vertex
};
// Vertex vertices[]{
//     {0.5, 0.5, 0, 1.0, 0.0, 0.0, 1.0},     // top vertex
//     {0.1, 0.1, 0.0, 1.0, 1.0, 0.0, 1.0},   // bottom left vertex
//     {0, 0, 0.0, 1.0, 0.0, 1.0, 1.0}     // bottom right vertex
// };
export namespace Pipelines{
    SDL_GPUGraphicsPipeline* pipelineA;
}

SDL_GPUBuffer* vertex_buffer;
SDL_GPUTransferBuffer* transfer_buffer;
SDL_GPUTransferBufferLocation transfer_location;
SDL_GPUBufferRegion copy_destination;
SDL_GPUColorTargetInfo gpu_color_target_info;

export void setupPipelines(){
    SDL_ClaimWindowForGPUDevice(gpu_device, sdl_window);

    Pipelines::pipelineA = setupPipelineA(gpu_device, sdl_window, (int)sizeof(Vertex));

    gpu_color_target_info = {
        .clear_color = {1.0f, 1.0f, 1.0f, 1.0f},//what color to clear the screen to
        .load_op = SDL_GPU_LOADOP_CLEAR,// or "SDL_GPU_LOADOP_LOAD" to keep the previous content
        .store_op = SDL_GPU_STOREOP_STORE,// store the content to the texture
    };

    //create a buffer on the gpu
    SDL_GPUBufferCreateInfo vertex_buffer_info{
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(vertices),//size of the buffer
        .props = 0,// propertie ID if need extension
    };
    vertex_buffer = SDL_CreateGPUBuffer(gpu_device, &vertex_buffer_info);//very expensive to create, reuse it

    //create a transfer buffer
    SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, //this buffer transfer data to the gpu
        // .size = (uint32_t)size_t(vertices),
        .size = sizeof(vertices),
        .props = 0,
    };
    transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_buffer_info);
    //the passing info about the transfer buffer
    transfer_location = {
        .transfer_buffer = transfer_buffer,
        .offset = 0,
    };

    //info about the destination of the copy pass, which is the buffer we created
    copy_destination = {
        .buffer = vertex_buffer,
        .offset = 0,// begin writing from the first vertex
        .size = sizeof(vertices),
    };
}

export void drawFrame(){
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu_device);
    

    //fill in the transfer buffer with verticies:
    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(gpu_device, transfer_buffer, false);
    data[0] = vertices[0];
    data[1] = vertices[1];
    data[2] = vertices[2];
    // SDL_memcpy(data, vertices, sizeof(vertices));//or do this, this copy the entire verticies array to the transfer
    SDL_UnmapGPUTransferBuffer(gpu_device, transfer_buffer);//clean up

    //upload to copy pass
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
    SDL_UploadToGPUBuffer(copyPass, &transfer_location, &copy_destination, true);
    SDL_EndGPUCopyPass(copyPass);

    //prepare the render pass
    SDL_GPUTexture* swapchainTexture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd, sdl_window, &swapchainTexture, &width, &height);
    if(swapchainTexture == NULL){return;}//can sometime false, for example: window minimized
    gpu_color_target_info.texture = swapchainTexture;// window's swapchain texture

    //pass info about drawing stuff onto screen
    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(cmd, &gpu_color_target_info, 1, NULL);
    SDL_BindGPUGraphicsPipeline(render_pass, Pipelines::pipelineA);//bind the
    SDL_GPUBufferBinding bufferBindings[1]{{
        .buffer = vertex_buffer,
        .offset = 0,
    }};
    SDL_BindGPUVertexBuffers(render_pass, 0, bufferBindings, 1);
    SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
    

    //clean up after every draw
    SDL_EndGPURenderPass(render_pass);
    SDL_SubmitGPUCommandBuffer(cmd);
}

//--experimental-modules-support
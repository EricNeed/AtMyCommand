#include<src/client/rendering/rendering.h>
#include<SDL3_shadercross/SDL_shadercross.h>

SDL_ShaderCross_HLSL_Info hi = {
    .source = "../src/shader/shader.hlsl",
    .entrypoint = "mainVSH",
    .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX,
};

ClientRendering::ClientRendering(){
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GPU Backend is: %s", SDL_GetGPUDeviceDriver(sdl_gpu_device));
    SDL_ClaimWindowForGPUDevice(sdl_gpu_device, sdl_window);
}

ClientRendering& ClientRendering::GetOnlyInstance(){
    static ClientRendering mainInstance;
    return mainInstance;
}

void ClientRendering::tickRendering(){
    static SDL_GPUShaderCreateInfo info = {

    };
    static SDL_GPUShader* shader_script = SDL_CreateGPUShader(sdl_gpu_device, );
}
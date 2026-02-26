#pragma once
#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>

class ClientRendering{
    private:
        SDL_GPUShaderFormat supported_formats = (
            SDL_GPU_SHADERFORMAT_SPIRV  | // For Vulkan
            SDL_GPU_SHADERFORMAT_MSL    | // For Metal (macOS/iOS)
            SDL_GPU_SHADERFORMAT_DXIL   | // For Direct3D 12
            SDL_GPU_SHADERFORMAT_DXBC   |
            SDL_GPU_SHADERFORMAT_METALLIB  
        );
        SDL_Window* sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE);
        SDL_GPUDevice* sdl_gpu_device = SDL_CreateGPUDevice(supported_formats, NULL, NULL);


        ClientRendering();
        ClientRendering(const ClientRendering&) = delete;
        ClientRendering& operator=(const ClientRendering&) = delete;

    public:
        static ClientRendering& GetOnlyInstance();
        void tickRendering();
};
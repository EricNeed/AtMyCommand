#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>
#include<SDL3_shadercross/SDL_shadercross.h>


SDL_GPUShaderFormat supported_formats = (
    SDL_GPU_SHADERFORMAT_SPIRV  | // For Vulkan
    SDL_GPU_SHADERFORMAT_MSL    | // For Metal (macOS/iOS)
    SDL_GPU_SHADERFORMAT_DXIL   | // For Direct3D 12
    SDL_GPU_SHADERFORMAT_DXBC   |
    SDL_GPU_SHADERFORMAT_METALLIB  
);

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
    
    SDL_Window* sdl_window = SDL_CreateWindow("AtMyCommand", 960, 540, SDL_WINDOW_RESIZABLE);
    SDL_GPUDevice* sdl_gpu_device = SDL_CreateGPUDevice(supported_formats, NULL, NULL);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GPU Backend is: %s", SDL_GetGPUDeviceDriver(sdl_gpu_device));
    SDL_ClaimWindowForGPUDevice(sdl_gpu_device, sdl_window);


    while(game_running){

        processSDLEvents();
    }
    return 0;
}
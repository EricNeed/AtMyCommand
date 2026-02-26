#include<src/client/rendering/rendering.h>

ClientRendering::ClientRendering(){
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GPU Backend is: %s", SDL_GetGPUDeviceDriver(sdl_gpu_device));
    SDL_ClaimWindowForGPUDevice(sdl_gpu_device, sdl_window);
}

ClientRendering& ClientRendering::GetOnlyInstance(){
    static ClientRendering mainInstance;
    return mainInstance;
}

void ClientRendering::tickRendering(){

}
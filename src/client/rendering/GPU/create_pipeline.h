#pragma once
#include<SDL3/SDL.h>

namespace CreateGPUPipeline{
    bool main(SDL_Window* window, SDL_GPUDevice* gpu_device, uint8_t profile_num);//profile_num: which pipeline in the "pipeline_info"
    bool createShader();
};
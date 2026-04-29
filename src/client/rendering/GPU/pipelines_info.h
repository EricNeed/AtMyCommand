#pragma once
#include<SDL3/SDL.h>
//this header file store all info about the pipelines used in this program

//general information about the pipeline
namespace PipelineInfogeneral{
    #ifdef CrossShaderFormat
    SDL_GPUShaderFormat supported_formats = CrossShaderFormat;
    #endif


};

struct Pipeline_Info{
    const char* vsh_dir;
    const char* fsh_dir;
};

struct Pipeline_Collection{
    const char* vsh_dir;
    SDL_GPUShaderCreateInfo vsh_info;
    const char* fsh_dir;
    SDL_GPUShaderCreateInfo fsh_info;
};
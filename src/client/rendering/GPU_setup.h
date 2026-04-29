#include<SDL3/SDL.h>

class SDLGPUSetup{
    private:

    //"vert_shader" and "frag_shader" receive shader, return true on success, false otherwise
    bool setupShaders(SDL_GPUShader* vert_shader, SDL_GPUShader* frag_shader);
    bool setupPipeline();

    public:

    SDL_Window* sdl_window;
    SDL_GPUDevice* gpu_device;

    SDLGPUSetup();
};
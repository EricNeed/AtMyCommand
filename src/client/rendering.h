#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>

class ClientRendering{
    ClientRendering();
    static ClientRendering& GetOnlyInstance();
};
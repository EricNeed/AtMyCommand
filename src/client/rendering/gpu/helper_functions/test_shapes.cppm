module;
#include "SDL3/SDL_rect.h"
#include <SDL3/SDL.h>

export module Test_Shapes;

SDL_Color blueColor = {3, 169, 244, 255};
SDL_Color pureRed = {255, 0, 0, 255};

export SDL_Point atlasSize = {2, 2};
export Uint32 zLayerCount = 2;
export SDL_Color testCube[]{
    {0,255,0,255}, pureRed, blueColor, blueColor, 
    blueColor, blueColor, blueColor, blueColor
};
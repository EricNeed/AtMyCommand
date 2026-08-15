module;
#include "SDL3/SDL_rect.h"
#include <SDL3/SDL.h>

export module Test_Shapes;

SDL_Color blueColor = {3, 169, 244, 255};

export SDL_Point atlasSize = {2, 2};
export Uint32 zLayerCount = 2;
export SDL_Color testCube[]{
    blueColor, blueColor, blueColor, blueColor, 
    blueColor, blueColor, blueColor, blueColor
};
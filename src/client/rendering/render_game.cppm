module;

#include <SDL3/SDL.h>

export module Tick_Render;

import Create_New_Pipeline;


export void setupClientRender(){
    setupPipelines();
}


export void tickRender(){
    drawFrame();
}

//"C_Cpp.intelliSenseEngine": "disabled",
//    "clangd.path": "D:\\Downloads\\clangd-windows-20.1.8\\clangd_20.1.8\\bin\\clangd.exe"
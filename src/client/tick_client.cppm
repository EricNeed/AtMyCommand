module;
export module Tick_Client;

import Tick_Render;

export void setupClient(){
    setupClientRender();
}

export void tickClient(){
    tickRender();
}
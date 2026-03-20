#include<SDL3/SDL.h>
#include"src/client/client_main.h"




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

ClientMain client_scripts;

int main(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game is Running");


    while(game_running){
        client_scripts.tickClient();
        processSDLEvents();
    }
    return 0;
}
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>

#include "dimensions.h"
#include "klondikeNames.h"
#include "Slot.h"
#include "Rendering.h"
#include "GameType.h"
#include "Controller.h"
#include <UI.h>

bool initializeGame(SDL_Window*& window, SDL_Renderer*& renderer){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
// Create window and renderer -> Maybe use RESIZABLE instead of FULLSCREEN?
    if (!SDL_CreateWindowAndRenderer("myWindowName", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == false) {
        return false;
    }

// Logical coordinates. Like a small scale model of the real size window. For consistent controls.
    SDL_SetRenderLogicalPresentation(renderer, dimensions::RENDWINX, dimensions::RENDWINY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetWindowFullscreen(window, true);
    return true;
}


int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* backGround = nullptr;
    int running = 1;
    SDL_Event event;
    

    if (!initializeGame(window,renderer)){
        return 1;
    }

    Controller control(renderer);
    Uint32 lastTick = SDL_GetTicks();

    while (running) {
        Uint32 currTick = SDL_GetTicks();
        float time = (currTick - lastTick) / 1000.0f;
        lastTick = currTick;
        while (SDL_PollEvent(&event)) {
            if (control.handleClick(event) == false){
                running = 0;
            }
        }
        float winX, winY;
        float mouseX, mouseY;
        SDL_GetMouseState(&winX, &winY);
        SDL_RenderCoordinatesFromWindow(renderer, winX, winY, &mouseX, &mouseY);
        control.updateGame(mouseX,mouseY,time);
        control.checkWin();
        control.Render();

        SDL_RenderPresent(renderer);
    }    
// Cleanup 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
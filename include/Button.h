#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>
#include "dimensions.h"
#include "UIAction.h"
#include <functional>


struct Button {
    SDL_Texture* text = nullptr;
    SDL_FRect rect {0,0,0,0};
    float textWidth = 0;
    float textHeight = 0;
    UIAction action = UIAction::none;
};

#endif
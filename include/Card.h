#ifndef CARD_H
#define CARD_H

#include <SDL3/SDL_rect.h>
#include "dimensions.h"

struct Card {
    SDL_FRect rect {0,180,dimensions::WIDTH,dimensions::HEIGHT}; 
    int suit;
    int rank;
    int atlas;
    int ID;
    bool faceUp = false;
};

#endif
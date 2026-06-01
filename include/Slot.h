#ifndef SLOT_H
#define SLOT_H

#include <SDL3/SDL_rect.h>
#include <vector>
#include "Card.h"

struct Slot {
    std::vector<Card*>cards; 
    SDL_FRect rect;

    virtual void arrange() = 0;
    virtual void subRank(){
        
    };
    virtual ~Slot() = default;
};

#endif
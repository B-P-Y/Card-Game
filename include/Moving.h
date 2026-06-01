#ifndef MOVING_H
#define MOVING_H

#include <SDL3/SDL_rect.h>
#include <vector>
#include "Card.h"
#include "Slot.h"

struct Moving{ 
    float offsetX = 0.0f; 
    float offsetY = 0.0f; 
    float gap;
    std::vector<Card*> draggedCards;
    Slot* base = nullptr;
};


#endif
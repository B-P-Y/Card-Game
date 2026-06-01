#ifndef STOCK_H
#define STOCK_H

#include <SDL3/SDL_rect.h>
#include <vector>
#include "Card.h"
#include "Slot.h"

struct Stock : public Slot {
    // Inherits:
    //    std::vector<Card*>cards; 
    //    SDL_FRect rect;
    
    //Only handle the top card.
    void arrange() override{
        for (Card* card : cards){
            card->rect.x = rect.x;
            card->rect.y = rect.y;
        }
        return;
    }


};

#endif
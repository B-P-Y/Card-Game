#ifndef TABLEAU_H
#define TABLEAU_H

#include <SDL3/SDL_rect.h>
#include <vector>
#include "Card.h"
#include "Slot.h"

struct Tableau : public Slot {
    // Inherits:
    //    std::vector<Card*>cards; 
    //    SDL_FRect rect;

    int hiddenCount = 0;
    void arrange() override{
        float yGap = dimensions::gap(cards.size());
        for(int i = 0; i< cards.size(); ++i){
            cards[i] -> rect.x = rect.x;
            cards[i] -> rect.y = rect.y + (i *yGap);
        }
    }



};

#endif
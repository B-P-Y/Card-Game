#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <SDL3/SDL_rect.h>
#include <vector>
#include "Card.h"
#include "Slot.h"

struct Foundation : public Slot {
    // Inherits:
    //    std::vector<Card*>cards; 
    //    SDL_FRect rect;

    //Club = 0, Heart = 1, Spade = 2, Diamond = 3.
    private:
    int currRank = 0;
    
    public:
    int suit= -1;

    void arrange() override{
        for(Card* card : cards){
            card->rect.x = rect.x;
            card->rect.y = rect.y;
        }
    }
    int getRank(){
        return currRank;
    }
    void addRank(){
        currRank += 1;
    }
    void subRank() override {
        currRank -= 1;
        if (currRank == 0){
            suit = -1;
        }
    }


};

#endif
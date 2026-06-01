#ifndef ANIMATION_H
#define ANIMATION_H
#include "Card.h"

struct Animation{ 
    Card* card;

    float targetX = 0.0f; 
    float targetY = 0.0f; 
    float speed = 0.0f;
};


#endif
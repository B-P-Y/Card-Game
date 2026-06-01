#include <SDL3/SDL.h>
#include "Animator.h"
#include "Animation.h"
#include <iostream>

Animator::Animator(){};


bool Animator::moveRect(Animation animate, float time){
    std::cout<<"Pass here"<<"\n";
    float destX = animate.targetX;
    float destY = animate.targetY;
    SDL_FRect& box = animate.card->rect;
    float speed = 1500;

    // Find how much it needs to move in the X and Y direction.
    float dx = destX - box.x;
    float dy = destY - box.y;
//Figure out the magnitude of the vector. 
    float distance = sqrtf(dx*dx + dy*dy);

    // If close enough, quit.
    if (distance <= 0.01f) return true;

// distance = velocity * time.
    float step = speed *time;

// If overshoot happens, snap to target.
    if (step >= distance){
        box.x = destX;
        box.y = destY;
        return true;
    }
// Normalize the vector to a unit vector -> Separates direction from speed.
    dy /= distance;
    dx /= distance;
//Add to current position.
    box.x += dx * step;
    box.y += dy * step;
    return false;

}

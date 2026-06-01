#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SDL3/SDL.h>
#include "Animation.h"

class Animator {
    Animator();
    public:
    static bool moveRect(Animation animate,float time);
};

#endif
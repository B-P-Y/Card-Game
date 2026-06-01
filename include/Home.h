#ifndef HOME_H
#define HOME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "Moving.h"
#include "Button.h"

class Rendering;
class Home {
public:
    Home();
    void init(Rendering& renderer); //create buttons and title
    void draw(Rendering& renderer); //tell renderer todraw buttons and title
    UIAction handleClick(float mouseX, float mouseY);


private:
    std::vector<Button> buttons;
    bool testClick(float mouseX, float mouseY, const SDL_FRect& rect);
    
    
};

#endif
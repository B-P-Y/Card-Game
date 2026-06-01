#include "Button.h"
#include <SDL3/SDL.h>
#ifndef UI_H
#define UI_H

class Rendering;
class UI {
    public:
        UI();

        void init(Rendering& renderer);
        UIAction UIClick(float mouseX, float mouseY);
        UIAction PauseClick(float mouseX, float mouseY);
        
        void drawUI(Rendering& renderer);
        void drawPause(Rendering& renderer);
        void drawWin(Rendering& renderer);

    private:
        SDL_FRect background;
        Button win;
        bool testClick(float mouseX, float mouseY, const SDL_FRect& rect);
        // Undo, Menu, 
        std::vector<Button> buttons;
        //Reset, New, Home
        std::vector<Button> pauseBtn;

};


#endif
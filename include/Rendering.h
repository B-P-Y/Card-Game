#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_ttf.h>
#include "Moving.h"
#include "Button.h"

class Rendering {
public:
    TTF_Font* fontSmall;
    TTF_Font* fontMed;
    TTF_Font* fontMenu;
    TTF_Font* fontLarge;
    Rendering(SDL_Renderer* renderer);
    ~Rendering();

    void loadAssets();
    void drawRect(const SDL_FRect& rect, SDL_Color colour, bool fill);
    void drawBackground();
    void drawSlot(const Slot& slot);
    void drawUI(int score, int time);
    void drawButton(const Button& curr);
    SDL_Texture* createText(const char* text, TTF_Font* font);
    void setColour(SDL_Color colour);
    void drawCardFace(const Card& card);
    void drawArrow(const SDL_FRect& box);


private:
    SDL_Renderer* renderer;
    SDL_Texture* atlases[2];
    SDL_Texture* background;
    SDL_Texture* cardBack;
    std::vector<SDL_Texture*> buttonTextures;

    SDL_Color textColour = {255,255,255,255};

    SDL_Texture* Time;
    SDL_Texture* Score;

    
    
    
};

#endif
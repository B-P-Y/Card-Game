#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include "Rendering.h"
#include <Button.h>


Rendering::Rendering(SDL_Renderer* renderer){
    Rendering::renderer = renderer;

};

Rendering::~Rendering(){
    if (background){
        SDL_DestroyTexture(background);
        background = nullptr;
    }
    for (int i = 0; i < 2; i++){
        if (atlases[i]){
            SDL_DestroyTexture(atlases[i]);
            atlases[i] = nullptr;
        }
    }
    for(SDL_Texture* texture : buttonTextures){
        SDL_DestroyTexture(texture);
    }
    buttonTextures.clear();
}


/*
TODO:
Consolidate the asset loading into one place. Currently have to keep on swapping between the two.
for debug / release
(low priority, doesn't affect the code.)


Replace cardBack.png and smiley.png with final versions. 
Especially cardBack.png, I'm not sure about copyright laws/rules and I didn't make that image.
*/
//For debug

void Rendering::loadAssets(){

    background = IMG_LoadTexture(renderer,"../../Assets/smiley.png");
    cardBack = IMG_LoadTexture(renderer,"../../Assets/cardBack.png");
    atlases[0] = IMG_LoadTexture(renderer,"../../Assets/Atlas0.png");
    atlases[1] = IMG_LoadTexture(renderer,"../../Assets/Atlas1.png");

    fontSmall = TTF_OpenFont("../../Assets/fonts/LMR.ttf", 16);
    fontMed = TTF_OpenFont("../../Assets/fonts/LMR.ttf", 24);
    fontMenu = TTF_OpenFont("../../Assets/fonts/LMR.ttf", 36);
    fontLarge = TTF_OpenFont("../../Assets/fonts/LMR.ttf", 72);

}

//for export -> Copy the asset folder over to a better location if needed.
// Currently needs to move the assets folder to the same location as the exe file
/*
void Rendering::loadAssets(){
    
    background = IMG_LoadTexture(renderer,"Assets/smiley.png");
    cardBack = IMG_LoadTexture(renderer,"Assets/cardBack.png");
    atlases[0] = IMG_LoadTexture(renderer,"Assets/Atlas0.png");
    atlases[1] = IMG_LoadTexture(renderer,"Assets/Atlas1.png");

    fontSmall = TTF_OpenFont("Assets/fonts/LMR.ttf", 16);
    fontMed = TTF_OpenFont("Assets/fonts/LMR.ttf", 24);
    fontMenu = TTF_OpenFont("Assets/fonts/LMR.ttf", 36);
    fontLarge = TTF_OpenFont("Assets/fonts/LMR.ttf", 72);
}
*/

//Create the texture here
SDL_Texture* Rendering::createText(const char* text, TTF_Font* font){
    SDL_Surface* surface = TTF_RenderText_Blended(font, text,strlen(text),textColour);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    buttonTextures.push_back(texture);
    return texture;
}


void Rendering::drawButton(const Button& curr){
    SDL_SetRenderDrawColor(renderer, 16, 42, 67, SDL_ALPHA_OPAQUE);
    SDL_FRect textRect;
    textRect.w = curr.textWidth;
    textRect.h = curr.textHeight;
    textRect.x = curr.rect.x + (curr.rect.w - textRect.w) / 2;
    textRect.y = curr.rect.y + (curr.rect.h - textRect.h) / 2;
    
    SDL_RenderFillRect(renderer, &curr.rect);
    SDL_RenderTexture(renderer,curr.text,NULL,&textRect);
}

void Rendering::drawUI(int score, int time){
    score = 0;
    time = 0;
}
//  1280, 720

void Rendering::drawSlot(const Slot& slot){
    SDL_FRect box = slot.rect;
    SDL_RenderTexture(renderer,background,NULL,&box);
}



void Rendering::drawArrow(const SDL_FRect& box){
    float centreY = box.y + box.h / 2.0f;
    float leftX   = box.x + 10;
    float rightX  = box.x + box.w - 10;

    float headSize = box.h / 4.0f;
//renderer, startx,starty,endx,endy.
    SDL_RenderLine(renderer, leftX,  centreY, rightX, centreY);
    SDL_RenderLine(renderer, leftX,  centreY, leftX  +headSize, centreY - headSize);
    SDL_RenderLine(renderer, leftX,  centreY, leftX  +headSize, centreY + headSize);
    SDL_RenderLine(renderer, rightX, centreY, rightX -headSize, centreY - headSize);
    SDL_RenderLine(renderer, rightX, centreY, rightX -headSize, centreY + headSize);
}

void Rendering::drawCardFace(const Card& card){
    if(!card.faceUp){
        SDL_RenderTexture(renderer,cardBack,NULL,&card.rect);
        return;
    }
    SDL_FRect textRect;
    int grid;
    textRect.w = dimensions::TEXTWIDTH;
    textRect.h = dimensions::TEXTHEIGHT;
    if (card.atlas == 0){
        grid = dimensions::ATLAS0;
    }
    else{
        grid = dimensions::ATLAS1;
    }
    textRect.x = dimensions::TEXTX +(card.ID % grid) * (dimensions::TEXTWIDTH + dimensions::TEXTX);
    textRect.y = dimensions::TEXTY + (card.ID / grid)* (dimensions::TEXTHEIGHT + dimensions::TEXTY);
    
    SDL_RenderTexture(renderer,atlases[card.atlas],&textRect, &card.rect);
}

void Rendering::drawRect(const SDL_FRect& rect, SDL_Color colour, bool fill){
    SDL_SetRenderDrawColor(renderer, colour.r,colour.g,colour.b, colour.a);
    if(fill == true){
        SDL_RenderFillRect(renderer, &rect);
    }
    else{
        SDL_RenderRect(renderer, &rect);
    }
}


void Rendering::drawBackground(){
    SDL_SetRenderDrawColor(renderer, 14, 122, 67, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}


void Rendering::setColour(SDL_Color colour){
    SDL_SetRenderDrawColor(renderer,colour.r,colour.g,colour.b,colour.a);
}
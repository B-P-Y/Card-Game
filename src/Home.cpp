#include"Home.h"
#include"Button.h"
#include "Rendering.h"


Home::Home(){}


void Home::init(Rendering& renderer) {

    Button Solitaire;
    Solitaire.text = renderer.createText("SOLITAIRE", renderer.fontMenu);
    SDL_GetTextureSize(Solitaire.text, &Solitaire.textWidth, &Solitaire.textHeight);
    Solitaire.rect = {200, 140, 360, 240};
    Solitaire.action = UIAction::StartSolitaire;
    buttons.push_back(std::move(Solitaire));

    Button klondike;
    klondike.text = renderer.createText("KLONDIKE",renderer.fontMenu);
    SDL_GetTextureSize(klondike.text, &klondike.textWidth, &klondike.textHeight);
    klondike.rect = {720,140,360,240};
    klondike.action = UIAction::StartKlondike;
    buttons.push_back(std::move(klondike));


    Button Pyramid;
    Pyramid.text = renderer.createText("PYRAMID",renderer.fontMenu);
    SDL_GetTextureSize(Pyramid.text,&Pyramid.textWidth,&Pyramid.textHeight);
    Pyramid.rect = {200,420,360,240};
    Pyramid.action = UIAction::StartPyramid;
    buttons.push_back(std::move(Pyramid));

    Button Freecell;
    Freecell.text = renderer.createText("FREECELL",renderer.fontMenu);
    SDL_GetTextureSize(Freecell.text,&Freecell.textWidth,&Freecell.textHeight);
    Freecell.rect = {720,420,360,240};
    Freecell.action = UIAction::StartFreecell;
    buttons.push_back(std::move(Freecell));
}

bool Home::testClick(float mouseX, float mouseY, const SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + box.w &&
        mouseY >= box.y &&
        mouseY <= box.y + box.h){
            return true;
    }
    return false;
}

UIAction Home::handleClick(float mouseX, float mouseY){
    for(const Button& button : buttons){
        if(testClick(mouseX,mouseY,button.rect)){
            return button.action;
        }
    }
    
    return UIAction::none;
}


void Home::draw(Rendering& renderer){
    renderer.drawBackground();
    for (const Button& button : buttons){
        renderer.drawButton(button);
    }
}

#include"UI.h"
#include"Button.h"
#include "Rendering.h"
#include "UIAction.h"
#include <iostream>


UI::UI(){}


void UI::init(Rendering& renderer) {
    Button undo;
    undo.text = renderer.createText("UNDO", renderer.fontSmall);
    SDL_GetTextureSize(undo.text, &undo.textWidth, &undo.textHeight);
    undo.rect = {32, 646, 72, 50};
    undo.action = UIAction::undo;
    buttons.push_back(std::move(undo));

    Button menu;
    menu.text = renderer.createText("Menu",renderer.fontSmall);
    SDL_GetTextureSize(menu.text, &menu.textWidth, &menu.textHeight);
    menu.rect = {1176,646,72,50};
    menu.action = UIAction::menu;
    buttons.push_back(std::move(menu));




    background = {507,280,266,160};
    win.text = renderer.createText("WIN!!!",renderer.fontMed);
    SDL_GetTextureSize(win.text,&win.textWidth,&win.textHeight);
    win.action = UIAction::NewGame;
    win.rect = {507,280,266,160};



    Button Back;
    Back.text = renderer.createText("BACK",renderer.fontMed);
    SDL_GetTextureSize(Back.text,&Back.textWidth,&Back.textHeight);
    Back.action = UIAction::back;
    pauseBtn.push_back(std::move(Back));

    Button Reset;
    Reset.text = renderer.createText("RESET",renderer.fontMed);
    SDL_GetTextureSize(Reset.text,&Reset.textWidth,&Reset.textHeight);
    Reset.action = UIAction::reset;
    pauseBtn.push_back(std::move(Reset));

    Button New;
    New.text = renderer.createText("NEW",renderer.fontMed);
    SDL_GetTextureSize(New.text,&New.textWidth,&New.textHeight);
    New.action = UIAction::NewGame;
    pauseBtn.push_back(std::move(New));

    Button Home;
    Home.text = renderer.createText("HOME",renderer.fontMed);
    SDL_GetTextureSize(Home.text,&Home.textWidth,&Home.textHeight);
    Home.action = UIAction::GoHome;
    pauseBtn.push_back(std::move(Home));

    Button Exit;
    Exit.text = renderer.createText("EXIT",renderer.fontMed);
    SDL_GetTextureSize(Exit.text,&Exit.textWidth,&Exit.textHeight);
    Exit.action = UIAction::exit;
    pauseBtn.push_back(std::move(Exit));


    for(int i = 0; i<pauseBtn.size(); i++){
        pauseBtn[i].rect.x = 472;
        pauseBtn[i].rect.y = 260 + (i*40);
        pauseBtn[i].rect.w = 336;
        pauseBtn[i].rect.h = 40;
    }
}

bool UI::testClick(float mouseX, float mouseY, const SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + box.w &&
        mouseY >= box.y &&
        mouseY <= box.y + box.h){
            return true;
    }
    return false;
}


UIAction UI::UIClick(float mouseX, float mouseY){
    for(const Button& button : buttons){
        if(testClick(mouseX,mouseY,button.rect)){
            return button.action;
        }
    }
    return UIAction::none;
}


UIAction UI::PauseClick(float mouseX, float mouseY){
    for(const Button& button : pauseBtn){
        if(testClick(mouseX,mouseY,button.rect)){
            return button.action;
        }
    }
    return UIAction::none;
}


void UI::drawUI(Rendering& renderer){
    for (const Button& button : buttons){
        renderer.drawButton(button);
    }
}

void UI::drawPause(Rendering& renderer){

    for (const Button& button : pauseBtn){
        renderer.drawButton(button);
    }
}

void UI::drawWin(Rendering& renderer){
    renderer.drawRect(background,{255,255,255, SDL_ALPHA_OPAQUE},true);
    renderer.drawButton(win);
    return;
}
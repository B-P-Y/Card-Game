#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Rendering.h"
#include "UI.h"
#include "Home.h"
#include "AppState.h"

class Controller {
public:
    Rendering rendering;
    std::unique_ptr<Game> currentGame;
    UI ui;
    GameType state;
    AppState appState;
    Home home;
    SDL_Renderer* renderer;


    Controller(SDL_Renderer* render);
    bool handleClick(SDL_Event& event);
    void changeState();
    void setUp();
    void initGame();
    void initMenu();
    void initUI();
    void initHandler();
    void initRender();
    void initHome();
    void updateGame(float mouseX, float mouseY,float time);
    void checkWin();
    void Render();

    private:
    void initState();

};

#endif
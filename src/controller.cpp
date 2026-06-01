#include "Controller.h"
#include "klondike.h"
#include "Pyramid.h"
#include "Spider.h"
#include "FreeCell.h"


Controller::Controller(SDL_Renderer* render)
    : renderer(render),
    rendering(render),
    ui(),
    state(GameType::none),
    appState(AppState::Home),
    home(){
        initRender();
        initUI();
        initGame();
        initHome();
    }

void Controller::changeState(){
}

bool Controller::handleClick(SDL_Event& event){
    float mouseX;
    float mouseY;
    SDL_RenderCoordinatesFromWindow(renderer, event.button.x, event.button.y, &mouseX, &mouseY);
    if(event.type == SDL_EVENT_QUIT){
        return false;
    }
    else if (event.type == SDL_EVENT_KEY_DOWN){
        if(event.key.key == SDLK_ESCAPE){
            return false;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        if(appState == AppState::Home){
            UIAction res = home.handleClick(mouseX,mouseY);
            switch (res) {
                case UIAction::StartKlondike:
                    appState = AppState::Playing;
                    state = GameType::klondike;
                    initGame();
                    break;
                case UIAction::StartSolitaire:
                    appState = AppState::Playing;
                    state = GameType::solitaire;
                    initGame();
                    break;
                case UIAction::StartPyramid:
                    appState = AppState::Playing;
                    state = GameType::pyramid;
                    initGame();
                    break;
                case UIAction::StartFreecell:
                    appState = AppState::Playing;
                    state = GameType::freecell;
                    initGame();
                    break;
                default:
                    break;
            }
            return true;
        }
        else if (appState == AppState::Playing){
            //Test for collisions with UI.
            UIAction res = ui.UIClick(mouseX,mouseY);
            if (res == UIAction::none){
                //Test for game collisions
                currentGame->mouseDown(mouseX,mouseY);
            }
            else if(res == UIAction::menu){
                appState = AppState::Paused;
            } 
            else if(res == UIAction::undo){
                currentGame->undo();
            }
            return true;
        }

        else if(appState == AppState::Paused){
            //events for menu only. 
            UIAction res = ui.PauseClick(mouseX,mouseY);

            switch (res) {
                case UIAction::back:
                    appState = AppState::Playing;
                    break;
                case UIAction::reset:
                    currentGame->Reset();
                    appState = AppState::Playing;
                    break;
                case UIAction::NewGame:
                    currentGame->New();
                    appState = AppState::Playing;
                    break;

                case UIAction::GoHome:
                    appState = AppState::Home;
                    state = GameType::none;
                    break;
                case UIAction::exit:
                    return false;
                    break;
                default:
                    break;
            }
        }
        /*
        TODO:
        Once the player wins the current game, they should be able to either start a new game,
        redo the current game, return to main menu or quit. 
        */
        else if(appState == AppState::win){
            //Setup the win state code here.
        }
    }
    else if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
        //Don't need to test for mouse up on UI elements, all interations are mouse down only.
        if (appState == AppState::Home){
            return true;
        }
        if(appState == AppState::Playing){
            currentGame->mouseUp(mouseX,mouseY);
        }
    }
    return true;
}

void Controller::updateGame(float mouseX, float mouseY, float time){
    if(appState == AppState::Playing){
        currentGame->update(mouseX,mouseY,time);
    }
}


void Controller::checkWin(){
    if(currentGame && currentGame->testWin() == true){
        appState = AppState::win;
    }
}


void Controller::Render(){
    if(appState == AppState::Playing){
        currentGame->drawGame(rendering);
        ui.drawUI(rendering);
    }
    else if(appState == AppState::Paused){
        ui.drawPause(rendering);
    }
    else if(appState == AppState::win){
        ui.drawWin(rendering);
    }
    else if(appState == AppState::Home){
        home.draw(rendering);
    }
}

//Check for dependencies in other parts of code. If no references, DELETE
void Controller::setUp(){

}

void Controller::initGame(){
    if(state == GameType::klondike){
        currentGame = std::make_unique<klondike>();
        currentGame->startGame();
    }
    else if (state == GameType::pyramid){
        currentGame = std::make_unique<Pyramid>();
        currentGame->startGame();
    }
    else if (state == GameType::solitaire){
        currentGame = std::make_unique<Spider>();
        currentGame->startGame();
    }
    else if (state ==GameType::freecell){
        currentGame = std::make_unique<freeCell>();
        currentGame->startGame();
    }
    return;
}


void Controller::initHome(){
    home.init(rendering);
}

void Controller::initUI(){
    ui.init(rendering);
}

void Controller::initRender(){
    rendering.loadAssets();
}


//TODO Setup or delete these init states. Decide later.
void Controller::initMenu(){
}

void Controller::initHandler(){
}

void Controller::initState(){
}

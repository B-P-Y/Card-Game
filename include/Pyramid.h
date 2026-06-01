#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include "Card.h"
#include "Game.h"
#include "Slot.h"
#include "Tableau.h"
#include "Stock.h"
#include "Foundation.h"
#include "dimensions.h"
#include "klondikeNames.h"
#include "Moving.h"
#include "UndoAction.h"
#include "GameType.h"
#include <random>
#include "Rendering.h"
#include "Selected.h"
#include <array>

class Pyramid :  public Game {
public:
    std::vector<Card> deck;
    std::vector<std::vector<Card*>> pyramid;
    Selected selected;
    std::vector<std::vector<bool>> live;
    Stock stock;
    Stock bin;
    Stock waste;
    int counter = 0;
    std::vector<std::vector<UndoAction>> history;
    std::mt19937 rng;
    SDL_FRect arrowBox;
    std::array<int, 4> arrow = {1,2,3,4};
    unsigned int seed;


    Pyramid();
    void startGame() override;
    void update(float mouseX, float mouseY,float time) override;
    void mouseDown(float mouseX, float mouseY) override;
    void mouseUp(float mouseX, float mouseY) override;
    void drawGame(Rendering& renderer) override;
    void Reset() override;
    void New() override;
    void undo() override;
    bool testWin() override;

    void initPyramid();
    void clearGame();
    void clearSelect();
    void handleMatch(Card* b,int i, int j);
    void handleMatch(Card*b, Stock& stock);
    UndoAction handleSelected();
    void selectCard(Card* card, Source source, int i, int j);
    void moveStock();
    bool testCollision(float mouseX, float mouseY, SDL_FRect& box);
    void shuffle(std::vector<Card>& cards);
    bool validMove();
    bool validPyramid();
};

#endif
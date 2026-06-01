#ifndef KLONDIKE_H
#define KLONDIKE_H

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

class klondike :  public Game {
public:
    std::vector<Card> deck;
    std::vector<Tableau> tableau;
    std::vector<Foundation> foundations;
    std::vector<UndoAction> history;
    std::mt19937 rng;
    unsigned int seed;
    Stock stock;
    Stock bin;
    Moving move;

    klondike();
    void startGame() override;
    void update(float mouseX, float mouseY,float time) override;
    void mouseDown(float mouseX, float mouseY) override;
    void mouseUp(float mouseX, float mouseY) override;
    void drawGame(Rendering& renderer) override;
    void Reset() override;
    void New() override;
    void undo() override;
    bool testWin() override;

    void initKlondike();
    void clearGame();
    void moveStock();
    bool testCollision(float mouseX, float mouseY, SDL_FRect& box);
    void movingCards(float mouseX, float mouseY);
    void moveCards(std::vector<Card*>& source, std::vector<Card*>& destination);
    void shuffle(std::vector<Card>& cards);
    bool validFoundation(std::vector<Card*>& source, Foundation& foundation, Slot* base);
    bool validTableauMove(std::vector<Card*>& source, std::vector<Card*>& destination, Slot* base);
    inline bool flipCard(Slot* base);
    

};

#endif
#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Card.h"
#include "Slot.h"
#include "Tableau.h"
#include "Stock.h"
#include "Foundation.h"
#include "dimensions.h"
#include "klondikeNames.h"
#include "Moving.h"
#include "UndoAction.h"
#include "GameType.h"
#include "Rendering.h"
#include <random>

class Game {
public:

    virtual ~Game() = default;
    virtual void startGame() = 0;
    virtual void update(float mouseX, float mouseY,float time) = 0;
    virtual void mouseDown(float mouseX, float mouseY) = 0;
    virtual void mouseUp(float mouseX, float mouseY) = 0;
    virtual void drawGame(Rendering& renderer) = 0;
    virtual void Reset() = 0;
    virtual void New() = 0;
    virtual void undo() = 0;
    virtual bool testWin() = 0;

};

#endif
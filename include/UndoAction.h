
#ifndef UNDOACTION_H
#define UNDOACTION_H

#include "Stock.h"
#include "UndoType.h"


struct UndoAction {
    UndoType type;
    //Pyramid only:
    std::pair<int,int> positions;


    Slot* origin;
    Slot* destination;
    

    int count;
    std::vector<Card*> toFlip;
    bool foundation;

    //Handle manual for pyramid and spider stock.
    UndoAction(UndoType t) : type(t) {}

    //Overload for klondike.
    UndoAction(UndoType t,Slot* o, Slot* d, int c = 1, std::vector<Card*> f = {}, bool found = false)
        : type(t), origin(o), destination(d), count(c),toFlip(f),foundation(found) {}
};


#endif

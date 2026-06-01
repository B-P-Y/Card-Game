#ifndef SELECTED_H
#define SELECTED_H

#include "Card.h"
#include "Slot.h"
#include "Source.h"

struct Selected{ 
    static constexpr int INVALID= -1; 
    int rank = -1;
    Card* card = nullptr;
    Source source = Source::None;
    int pyramidi = -1;
    int pyramidj = -1;
};


#endif
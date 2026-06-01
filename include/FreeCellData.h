#include <algorithm>
#ifndef FREECELLDATA_H
#define FREECELLDATA_H
namespace FreeCellData{
    inline constexpr int CARDCOUNT = 52;
    inline constexpr int FOUNDATION = 4;
    inline constexpr int TABLEAU = 8;
    inline constexpr int CELL = 4;

    inline constexpr int CELLX = 105;
    inline constexpr int CELLY = 70;

    inline constexpr int FOUNDATIONY = 70;
    inline constexpr int FOUNDATIONX = 765;

    inline constexpr int TABX = 145;
    inline constexpr int TABY = 240;

    inline constexpr int SPACING = 50;
    
    inline constexpr int WIDTH = 80;
    inline constexpr int HEIGHT = 120;

    inline constexpr int MAX_STACK_HEIGHT = 500;
    inline constexpr float PREFERRED_GAP = 30;


    constexpr float gap(int cardCount){
        if (cardCount <= 1) return 0.0f;
        float MAXGAP = (MAX_STACK_HEIGHT-HEIGHT) / float(cardCount - 1);
        return std::min(PREFERRED_GAP,MAXGAP);
    }

}
#endif

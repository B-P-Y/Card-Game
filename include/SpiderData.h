#include <algorithm>
#ifndef SPIDERDATA_H
#define SPIDERDATA_H
namespace SpiderData{
    inline constexpr int CARDCOUNT = 104;
    inline constexpr int FOUNDATION = 8;
    inline constexpr int TABLEAU = 10;

    inline constexpr int FOUNDATIONY = 24;
    inline constexpr int FOUNDATIONX = 280;
    inline constexpr int STOCKX = 40;
    inline constexpr int STOCKY = 24;
    inline constexpr int SPACING = 40;
    inline constexpr int TABY = 160;

    inline constexpr int WIDTH = 80;
    inline constexpr int HEIGHT = 120;

    inline constexpr int MAX_STACK_HEIGHT = 500;
    inline constexpr float PREFERRED_GAP = 20;


    constexpr float gap(int cardCount){
        if (cardCount <= 1) return 0.0f;
        float MAXGAP = (MAX_STACK_HEIGHT-HEIGHT) / float(cardCount - 1);
        return std::min(PREFERRED_GAP,MAXGAP);
    }

}
#endif

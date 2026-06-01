#include <algorithm>
#ifndef DIMENSIONS_H
#define DIMENSIONS_H


namespace dimensions{
// Render window stuff
    inline constexpr int RENDWINX = 1280;
    inline constexpr int RENDWINY = 720;
// Main Menu stuff
    inline constexpr int MAINBUTTONW = 200;
    inline constexpr int MAINBUTTONH = 120;
// Card stuff
    inline constexpr int WIDTH = 120; //80, 120
    inline constexpr int HEIGHT = 180; //120, 180
    inline constexpr int SMALLWIDTH = 80;
    inline constexpr int SMALLHEIGHT = 120;

// Vertical space for cards
    inline constexpr int MAX_STACK_HEIGHT = 500;



// Foundation stuff
    inline constexpr int FOUNDATIONY = 7;
    inline constexpr int FOUNDATIONX = 580; //560, 580
    inline constexpr int FOUNDATONSPACING = 40;

// tableau stuff
    inline constexpr int TABY = 194;
    inline constexpr int TABSPACING = 40; //40
    inline constexpr int TABX = 100;  //100 , 200

// Stock stuff
    inline constexpr int STOCKX = 100; // 100, 200
    inline constexpr int STOCKY = 7;
    inline constexpr int STOCKSPACING = 40;


// Gap stuff
    inline constexpr float PREFERRED_GAP = 30;

//Atlas stuff
    inline constexpr int TEXTWIDTH = 256;
    inline constexpr int TEXTHEIGHT = 384;
    inline constexpr int ATLAS0 = 7;
    inline constexpr int ATLAS1 = 6;
    inline constexpr int TEXTX = 2;
    inline constexpr int TEXTY = 8;
    inline constexpr int ATLASCOUNT = 34;

    constexpr float gap(int cardCount){
        if (cardCount <= 1) return 0.0f;
        float MAXGAP = (MAX_STACK_HEIGHT-HEIGHT) / float(cardCount - 1);
        return std::min(PREFERRED_GAP,MAXGAP);
    }
}

#endif
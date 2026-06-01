#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include "Pyramid.h"
#include "UndoAction.h"
#include "PyramidData.h"


Pyramid::Pyramid(){startGame();}


bool Pyramid::testWin(){
    for(int i = 0; i < 7; i++){
        for (int j = 0; j<=i; j++){
            if (live[i][j] == true) return false;
        }
    }
    return true;
}


void Pyramid::update(float mouseX, float mouseY,float time){
    return;
}
// No flipping cards in this game mode.
void Pyramid::undo(){
    if(history.size() == 0) return;
    std::vector<UndoAction> actions = history.back();
    while (!actions.empty()){
        UndoAction action = actions.back(); 
        if(action.type == UndoType::resetLive){
            int i = action.positions.first;
            int j = action.positions.second;
            live[i][j] = true;
        }
        else{
            if(action.type == UndoType::moveCounter){
                counter -= 1;
            }
            std::vector<Card*>& ori = action.origin->cards;
            std::vector<Card*>& dest = action.destination->cards;
            ori.insert(ori.end(),dest.end()-action.count,dest.end());
            dest.erase(dest.end()-action.count,dest.end());
            action.origin->arrange();
            action.destination->arrange();
        }
        actions.pop_back();
    }
    stock.arrange();
    bin.arrange();
    history.pop_back();
}

bool Pyramid::testCollision(float mouseX, float mouseY, SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + dimensions::SMALLWIDTH &&
        mouseY >= box.y &&
        mouseY <= box.y + dimensions::SMALLHEIGHT){
            return true;
    }
    return false;
}

void Pyramid::shuffle(std::vector<Card>& cards) {
    for (int i = 0; i < cards.size(); i++) {
        std::uniform_int_distribution<int> dist(i, cards.size() - 1);
        int j = dist(rng);
        std::swap(cards[i], cards[j]);
    }
}



void Pyramid::moveStock(){
    //move from stock to bin
    if (!stock.cards.empty()){
        Card* curr = stock.cards.back();
        bin.cards.push_back(curr);
        stock.cards.pop_back();
        UndoAction action = {UndoType::moveCards,&stock,&bin,1};
        history.emplace_back(1,action);

    }
    //move from bin to stock
    else{
        for (size_t i = bin.cards.size(); i-- > 0; ) {
            Card* curr = bin.cards[i];
            stock.cards.push_back(curr);
        }
        counter += 1;
        UndoAction action = {UndoType::moveCounter,&bin,&stock,static_cast<int>(stock.cards.size())};
        history.emplace_back(1,action);
        bin.cards.clear();
        
    }
    stock.arrange();
    bin.arrange();
}

void Pyramid::clearSelect(){
    if(selected.card){
        selected.card->rect.x += 10;
        selected.card->rect.y += 10;
        selected.card->rect.w -= 20;
        selected.card->rect.h -= 20;
    }
    selected = {};
}

void Pyramid::selectCard(Card* card,Source source, int i, int j){
    selected.rank  = card->rank;
    selected.card = card;
    selected.source = source;
    selected.pyramidi = i;
    selected.pyramidj = j;
    card->rect.x -= 10;
    card->rect.y -= 10;
    card->rect.w += 20;
    card->rect.h += 20;
}

UndoAction Pyramid::handleSelected(){
    if (selected.source == Source::Pyramid){
        UndoAction action = UndoType::resetLive;
        action.positions = {selected.pyramidi,selected.pyramidj};
        live[selected.pyramidi][selected.pyramidj] = false;
        return action;
    }
    else if(selected.source == Source::Bin) {
        bin.cards.pop_back();
        waste.cards.push_back(selected.card);
        UndoAction action = {UndoType::moveCards,&bin,&waste,1};
        return action;
    }
        
    else{
        stock.cards.pop_back();
        waste.cards.push_back(selected.card);
        UndoAction action = {UndoType::moveCards,&stock,&waste,1};
        return action;
    }
}

void Pyramid::handleMatch(Card* b,int i, int j){
    std::vector<UndoAction> actions;
    
    if(selected.rank!=Selected::INVALID){
        UndoAction action = handleSelected();
        actions.emplace_back(action);
    }
    live[i][j] = false;
    UndoAction action = UndoType::resetLive;
    action.positions = {i,j};
    actions.emplace_back(action);

    history.push_back(actions);
    clearSelect();
    return;

}
void Pyramid::handleMatch(Card* b, Stock& stock){
    std::vector<UndoAction> actions;
    //Handling selected.
    if(selected.rank != Selected::INVALID){
        UndoAction action = handleSelected();
        actions.emplace_back(action);
    }
    //Handle the stock
    stock.cards.pop_back();
    waste.cards.push_back(b);
    UndoAction action = {UndoType::moveCards,&stock,&waste,1};
    actions.emplace_back(action);
        
    history.push_back(actions);
    clearSelect();
    return;

}

void Pyramid::mouseDown(float mouseX,float mouseY){
    SDL_FRect box = arrowBox;
    //The arrow handles movement between the bin and the stock.
    if(testCollision(mouseX,mouseY,box)){
        if (counter < 3){
            moveStock();
            clearSelect();
        }
        return;
    }
    //Handle the stock
    box = stock.rect;
    if(!stock.cards.empty() && testCollision(mouseX,mouseY,box)){
        if(selected.rank == Selected::INVALID){
            if(stock.cards.back()->rank == 12){
                Card* card = stock.cards.back();
                handleMatch(card,stock);
            }
            else{
                selectCard(stock.cards.back(),Source::Stock,Selected::INVALID,Selected::INVALID);
            } 
        }
        else{
            Card* card = stock.cards.back();
            if(selected.rank + card->rank == 11){
                handleMatch(card,stock);
            }
            else{
            }
            clearSelect();
        }
        return;
    }

    //Handle the bin
    box = bin.rect;    
    if(!bin.cards.empty() && testCollision(mouseX,mouseY,box)){
        if(selected.rank == Selected::INVALID){
            if(bin.cards.back()->rank == 12){
                Card* card = bin.cards.back();
                handleMatch(card,bin);
            }
            else{
                selectCard(bin.cards.back(),Source::Bin,Selected::INVALID,Selected::INVALID);
            } 
        }
        else{

            Card* card = bin.cards.back();
            if(selected.rank + card->rank == 11){
                handleMatch(card,bin);
            }
            clearSelect();
        }
        return;
    }
//Handle the pyramid
    for(int i = 0; i<7; i++){
        for(int j = 0; j<=i; j++){
            if(testCollision(mouseX,mouseY,pyramid[i][j]->rect)){
                if(live[i][j] && !live[i+1][j] && !live[i+1][j+1]){
                    Card* card = pyramid[i][j];
                    if(selected.rank != Selected::INVALID){
                        if(selected.rank + card->rank == 11){
                            handleMatch(card,i,j);
                            return;
                        }
                        clearSelect();
                    }
                    else{
                        if (card->rank == 12){
                            handleMatch(card,i,j);
                        }
                        else{
                            selectCard(card,Source::Pyramid,i,j);
                        }
                        return; 
                    }
                }
                clearSelect();
                return;
            }
        }
    }
}

void Pyramid::mouseUp(float mouseX, float mouseY){
    return; //does nothing.
}

void Pyramid::Reset(){
    rng.seed(seed);
    clearGame();
    initPyramid();
}

void Pyramid::New(){
    startGame();
}

void Pyramid::startGame(){
    clearGame();
    seed = std::random_device{}();
    rng.seed(seed);
    initPyramid();
}

void Pyramid::clearGame(){
    deck.clear();
    for(std::vector<Card*>& Row : pyramid){
        Row.clear();
    }
    for(std::vector<bool>& Row : live){
        Row.clear();
    }
    selected = {};
    bin.cards.clear();
    stock.cards.clear();
    history.clear();
    counter = 0;
}

void Pyramid::drawGame(Rendering& renderer){
    renderer.drawBackground();
    for(int i = 0; i<7; i++){
        for(int j = 0; j<=i; j++){
            if(live[i][j] == true){
                renderer.drawCardFace(*pyramid[i][j]);
            }
            
        }
    }
    renderer.drawRect(stock.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
    renderer.drawRect(bin.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
    renderer.drawRect(arrowBox,{0,255,0,SDL_ALPHA_OPAQUE},false);
    renderer.drawArrow(arrowBox);
    for (const Card* card : stock.cards){
        renderer.drawCardFace(*card);
    }
    for (const Card* card:bin.cards){
        renderer.drawCardFace(*card);
    }
}

void Pyramid::initPyramid(){
    counter = 0;
    deck.resize(52);
    pyramid.resize(7);
    for(int i = 0; i<7; i++){
        pyramid[i].resize(i+1);
    }
    live.resize(8);
    for(int i = 0; i<8; i++){
        live[i].resize(i+1);
        for(int j = 0; j<=i; j++){
            live[i][j] = (i !=7);
        }
    }
    

    //Set up the card attributes.
    //Club = 0, Heart = 1, Spade = 2, Diamond = 3.
    for(int i = 0; i<klondikeNames::CARDCOUNT; i++){
        Card& card = deck[i];
        card.rank = (i % 13); // 0 -> 12
        card.suit = i / 13;
        card.rect.w = dimensions::SMALLWIDTH;
        card.rect.h = dimensions::SMALLHEIGHT;
        if (i<=dimensions::ATLASCOUNT){
            card.atlas = 0;
            card.ID = i;
        }
        else{
            card.atlas = 1;
            card.ID = i - 35;
        }
    }
    shuffle(deck);

    int curr = 0;
    for (int i = 0; i<7; i++){
        for (int j = 0; j <=i; j++){
            pyramid[i][j] = &deck[curr];
            deck[curr].faceUp = true;
            int baseX = dimensions::RENDWINX/2 - ((dimensions::SMALLWIDTH*(i+1) + PyramidData::XGAP*(i))/2);
            deck[curr].rect.y = 20 + (PyramidData::YGAP*i);
            deck[curr].rect.x = baseX + j * (dimensions::SMALLWIDTH + PyramidData::XGAP);
            curr += 1;
        }
    }
    for (curr; curr < PyramidData::CARDCOUNT; curr++){
        deck[curr].rect.x = stock.rect.x;
        deck[curr].rect.y = stock.rect.y;
        stock.cards.push_back(&deck[curr]);
        deck[curr].faceUp = true;
    }
    stock.rect.x = PyramidData::STOCKX;
    stock.rect.y = PyramidData::STOCKY;
    stock.rect.w = dimensions::SMALLWIDTH;
    stock.rect.h = dimensions::SMALLHEIGHT;


    arrowBox.x = PyramidData::STOCKX + dimensions::SMALLWIDTH + PyramidData::STOCKGAP;
    arrowBox.y = 580;
    arrowBox.w = dimensions::SMALLWIDTH;
    arrowBox.h = dimensions::SMALLHEIGHT / 2;

    bin.rect.x = arrowBox.x + dimensions::SMALLWIDTH + PyramidData::STOCKGAP;
    bin.rect.y = PyramidData::STOCKY;
    bin.rect.w = dimensions::SMALLWIDTH;
    bin.rect.h = dimensions::SMALLHEIGHT;
}
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include "Spider.h"
#include "SpiderData.h"
#include "UndoAction.h"
#include "Animation.h"
#include "Animator.h"

// WIN CONDITION NOT TESTED, I HAVE NOT YET BEATEN A SPIDER SOLIATIRE GAME.
// ANIMATIONS MOVED TO ANIMATOR.CPP 

Spider::Spider(){startGame();}

void Spider::moveCards(std::vector<Card*>& source, std::vector<Card*>& destination){
    destination.insert(destination.end(),source.begin(),source.end());
    source.clear();
}

//Win condition is when there are no cards left in the tableaus.
bool Spider::testWin(){
    for (Tableau &tab : tableau){
        if(!tab.cards.empty()){
            return false;
        }
    }
    return true;
}
 
//Handle the zoom in function for card being hovered over if the cards are too small

void Spider::update(float mouseX, float mouseY,float time){
    movingCards(mouseX, mouseY);
    for(int i = animate.size() - 1; i >= 0; --i){
        bool finished = Animator::moveRect(animate[i],time);
        if (finished) { animate.erase(animate.begin() + i); }
    }
}

void Spider::undo(){
    if(history.size() == 0) return;
    UndoAction action = history.back();
    if(action.type == UndoType::spiderStock){
        for(int i = 9; i> -1; i--){
            Card* card = tableau[i].cards.back();
            card->faceUp = false;
            card->rect = stock.rect;
            stock.cards.push_back(card);
            tableau[i].cards.pop_back();
            tableau[i].arrange();
        }
    }
    else{
        for(Card* card : action.toFlip){
            if (card->faceUp == true){
            }
            else{
            }
            card->faceUp = !card->faceUp; //Flip the faces
        }
        std::vector<Card*>& ori = action.origin->cards;
        std::vector<Card*>& dest = action.destination->cards;
        ori.insert(ori.end(),dest.end()-action.count,dest.end());

        dest.erase(dest.end()-action.count,dest.end());
        action.origin->arrange();
        action.destination->arrange();
        if(action.foundation){
            action.destination->subRank();
        }
    }
    history.pop_back();
}

bool Spider::testCollision(float mouseX, float mouseY, SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + box.w &&
        mouseY >= box.y &&
        mouseY <= box.y + box.h){
            return true;
    }
    return false;
}

void Spider::shuffle(std::vector<Card>& cards) {
    for (int i = 0; i < cards.size(); i++) {
        std::uniform_int_distribution<int> dist(i, cards.size() - 1);
        int j = dist(rng);
        std::swap(cards[i], cards[j]);
    }
}

/*
When cards are moved, first store the current location.
Then use the arrange function to dictate the destination.
Then add the cards into an animation pile.
Then call the check the pile and call it at the start of the draw phase.
*/
void Spider::moveStock(){
    if(stock.cards.empty()) return;
    for(int i = 0; i < 10; i++){
        Card* card = stock.cards.back();
        card->faceUp = true;
        tableau[i].cards.push_back(card);
        stock.cards.pop_back();
        SDL_FRect temp = card->rect;
        tableau[i].arrange();
        Animation animation;
        animation.card = card;
        animation.targetX = card->rect.x;
        animation.targetY = card->rect.y;
        animate.push_back(animation);
        card->rect = temp;
    }

    UndoAction action = UndoType::spiderStock;
    history.push_back(action);
    return;
}


inline bool Spider::flipCard(Slot* base){
    if (base->cards.size() != 0 &&
        base->cards.back()->faceUp == false){
        base->cards.back()->faceUp = true;
        return true;
    }
    return false;
}



bool Spider::validTableauMove(std::vector<Card*>& source, std::vector<Card*>& destination){
    if(destination.size()==0){
        if (source.front()->rank == 12){
            return true;
        }
        return false;
    }
    if(destination.back()->rank == source.front()->rank + 1){
            return true;
    }
    return false;

}


/*
Make sure the foundation is empty.
Find the suit for the first card. Make sure all other cards in the source are the same.
Loop over each card and make sure it increases in rank correctly. Starting at 0 and ending at 12.
*/
bool Spider::validFoundation(std::vector<Card*>& source, Foundation& foundation){
    if(!foundation.cards.empty()) return false;
    int suit = source.front()->suit;
    int rank = 0;
    for(Card* &card : source){
        if(card->suit != suit || card->rank != rank) return false;
        rank += 1;
    }
    if(rank != 13){
        return false;
    }
    return true;
}


void Spider::movingCards(float mouseX, float mouseY){
    if (!move.draggedCards.empty()) {
        for (int i = 0; i < move.draggedCards.size(); i++){
            move.draggedCards[i]->rect.x = mouseX -move.offsetX;
            move.draggedCards[i]->rect.y = (mouseY - move.offsetY) + (i * move.gap);
        }
    }
}

void Spider::mouseDown(float mouseX,float mouseY){

    if(testCollision(mouseX,mouseY,stock.rect)){
        moveStock();
        return;
    }
    //Check if the tab is selected.
    //Initialize by selecting the suit and rank based off last card.
    //For each card, suit must be the same and rank must be +1.
    //If not, break the loop.
    //If yes continue to hit check.
    for (Tableau &tab : tableau){
            bool overlap = 
            mouseX >= tab.rect.x && 
            mouseX <= tab.rect.x + dimensions::WIDTH;
            if (overlap){
                int suit = tab.cards.back()->suit;
                int rank = tab.cards.back()->rank - 1;
                for (int i = tab.cards.size() - 1; i >= 0; --i){
                    Card* myCard = tab.cards[i];
                    if(myCard->faceUp==false || myCard->rank != rank+1 || myCard->suit != suit) return;
                    overlap = 
                    mouseY >= myCard->rect.y &&
                    mouseY <= myCard->rect.y +dimensions::HEIGHT;
                    if (overlap){
                        move.draggedCards.assign(
                            tab.cards.begin() + i,
                            tab.cards.end()
                        );
                        tab.cards.erase(
                            tab.cards.begin() + i,
                            tab.cards.end()
                        );
                        move.offsetX = mouseX - myCard->rect.x;
                        move.offsetY = mouseY - myCard->rect.y;
                        move.base = &tab;
                        move.gap = dimensions::gap(move.draggedCards.size());
                        break;
                    }
                    rank += 1;
                }
                tab.arrange();
            }
        }
}


void Spider::mouseUp(float mouseX, float mouseY){
    if (move.draggedCards.empty()) return;
        //Check if user clicked on foundation.
        for (Foundation &found : foundations){
            SDL_FRect box = found.rect;
            if(testCollision(mouseX,mouseY,box)){
                if(validFoundation(move.draggedCards,found)){
                    int size = static_cast<int>(move.draggedCards.size());
                    moveCards(move.draggedCards,found.cards);

                    std::vector<Card*> temp = {};
                    if(flipCard(move.base)){
                        temp.push_back(move.base->cards.back());
                    }
                    UndoAction action = {UndoType::moveCards,move.base,&found,size,temp,true};
                    history.push_back(action);
                    found.addRank();
                }
                found.arrange();
                break;
            }
        }
        
        
        //Check if user clikced on tableaus
        for (Tableau &tab :tableau){
            SDL_FRect box = tab.rect;
            if (!tab.cards.empty()){
                SDL_FRect back = tab.cards.back()->rect;
                box = {box.x,box.y,box.w,back.y + back.h - box.y};
            }
                     
            if(testCollision(mouseX,mouseY,box)){
                if(validTableauMove(move.draggedCards,tab.cards)){
                    int size = static_cast<int>(move.draggedCards.size());
                    moveCards(move.draggedCards,tab.cards);
                    std::vector<Card*> temp = {};
                    if(flipCard(move.base)){
                        temp.push_back(move.base->cards.back());
                    }
                    UndoAction action = {UndoType::moveCards,move.base,&tab,size,temp,true};
                    history.push_back(action);
                }
                
                tab.arrange();
                break;
            }
        }
        if (!move.draggedCards.empty()){
            moveCards(move.draggedCards,move.base -> cards);
            move.base->arrange();
        }
    return;
}

void Spider::Reset(){
    rng.seed(seed);
    clearGame();
    initSpider();
}
void Spider::New(){
    startGame();
}


void Spider::startGame(){
    clearGame();
    seed = std::random_device{}();
    rng.seed(seed);
    initSpider();
}
void Spider::clearGame(){
    deck.clear();
    tableau.clear();
    foundations.clear();
    move.draggedCards.clear();
    stock.cards.clear();
    history.clear();
    animate.clear();
}

void Spider::drawGame(Rendering& renderer){
    renderer.drawBackground();
    for (const Slot &slot : tableau){
        int count = 0;
        renderer.drawRect(slot.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
        for (const Card* card : slot.cards){
            renderer.drawCardFace(*card);
            count += 1;
        }
    }

    for (const Foundation &foundation :foundations){
        renderer.drawRect(foundation.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
        if (!foundation.cards.empty()){
            renderer.drawCardFace(*foundation.cards.back()); //only draw the last card.
        }
        
    }
    renderer.drawRect(stock.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);


    for (const Card* card : stock.cards){
        renderer.drawCardFace(*card);
    }

    for (const Card* card : move.draggedCards){
        renderer.drawCardFace(*card);
    }

}


void Spider::initSpider(){
    static int count = 0;
    count++;

    deck.resize(SpiderData::CARDCOUNT);
    tableau.resize(SpiderData::TABLEAU);
    foundations.resize(SpiderData::FOUNDATION);
    move.draggedCards.reserve(13);

    //Set up the card attributes.
    //Club = 0, Heart = 1, Spade = 2, Diamond = 3.
    for(int i = 0; i<SpiderData::CARDCOUNT; i++){  
        Card& card = deck[i];
        card.rank = (i % 13); // 0 -> 12
        card.suit = (i % 52) / 13;
        card.rect.w = SpiderData::WIDTH;
        card.rect.h = SpiderData::HEIGHT;
        if ((i % 52)<=dimensions::ATLASCOUNT){
            card.atlas = 0;
            card.ID = i % 52;
        }
        else{
            card.atlas = 1;
            card.ID = (i % 52) - 35;
        }
    }

    shuffle(deck);


// Coordinates for the tableau. 
    for (int i = 0; i<SpiderData::TABLEAU; i++){
        int Xpos = SpiderData::SPACING + i * (SpiderData::WIDTH + SpiderData::SPACING);
        tableau[i].rect.x = Xpos;
        tableau[i].rect.y = SpiderData::TABY;
        tableau[i].rect.w = SpiderData::WIDTH;
        tableau[i].rect.h = SpiderData::HEIGHT;
    }
    
//foundations
    for (int i = 0; i<SpiderData::FOUNDATION; i++){
        int Xpos = SpiderData::FOUNDATIONX + i * (SpiderData::WIDTH + SpiderData::SPACING);
        foundations[i].rect.x = Xpos;
        foundations[i].rect.y = SpiderData::FOUNDATIONY;
        foundations[i].rect.w = SpiderData::WIDTH;
        foundations[i].rect.h = SpiderData::HEIGHT;
    }
//Stock
    stock.rect.x = SpiderData::STOCKX;
    stock.rect.y = SpiderData::STOCKY;
    stock.rect.w = SpiderData::WIDTH;
    stock.rect.h = SpiderData::HEIGHT;

// Set up the cards in the tableau.
    int curr = 0;
    for (int i = 0; i< 4; i++){
        for (int j = 0; j < 6; j++){
            deck[curr].rect.x = tableau[i].rect.x;
            deck[curr].rect.y = tableau[i].rect.y + (j * SpiderData::gap(j+1));
            tableau[i].cards.push_back(&deck[curr]);
            curr += 1;
        }
        tableau[i].cards.back()->faceUp = true;
    }
    for (int i = 4; i< 10; i++){
        for (int j = 0; j < 5; j++){
            deck[curr].rect.x = tableau[i].rect.x;
            deck[curr].rect.y = tableau[i].rect.y + (j * SpiderData::gap(j+1));
            tableau[i].cards.push_back(&deck[curr]);
            curr += 1;
        }
        tableau[i].cards.back()->faceUp = true;
    }


    for (curr; curr < SpiderData::CARDCOUNT; curr++){
        deck[curr].rect.x = stock.rect.x;
        deck[curr].rect.y = stock.rect.y;
        stock.cards.push_back(&deck[curr]);
    }
}
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include "klondike.h"
#include "UndoAction.h"


klondike::klondike(){startGame();}

void klondike::moveCards(std::vector<Card*>& source, std::vector<Card*>& destination){
    destination.insert(destination.end(),source.begin(),source.end());
    source.clear();
}

bool klondike::testWin(){
    int hidden = 0;
    for (const Tableau &tab: tableau){
        for (Card* c : tab.cards){
            if (c->faceUp == false){
                hidden += 1;
            }
        }
    }
    hidden += stock.cards.size();
    if (hidden == 0 && bin.cards.size() == 0){
        return true;
    }
    return false;
}
void klondike::update(float mouseX, float mouseY,float time){
    movingCards(mouseX, mouseY);
}
void klondike::undo(){
    if(history.size() == 0) return;
    UndoAction action = history.back(); 
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
    history.pop_back();
}

bool klondike::testCollision(float mouseX, float mouseY, SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + box.w &&
        mouseY >= box.y &&
        mouseY <= box.y + box.h){
            return true;
    }
    return false;
}

void klondike::shuffle(std::vector<Card>& cards) {
    for (int i = 0; i < cards.size(); i++) {
        std::uniform_int_distribution<int> dist(i, cards.size() - 1);
        int j = dist(rng);
        std::swap(cards[i], cards[j]);
    }
}

void klondike::moveStock(){
    //move from stock to bin
    if (!stock.cards.empty()){
        Card* curr = stock.cards.back();
        curr->faceUp = true;
        bin.cards.push_back(curr);
        stock.cards.pop_back();
        UndoAction action = {UndoType::moveCards,&stock,&bin,1,{curr}};
        history.push_back(action);

    }
    //move from bin to stock
    else{
        std::vector<Card*> temp = {};
        for (size_t i = bin.cards.size(); i-- > 0; ) {
            Card* curr = bin.cards[i];
            curr->faceUp = false;
            temp.push_back(curr);
            stock.cards.push_back(curr);

        }

        UndoAction action = {UndoType::moveCards,&bin,&stock,static_cast<int>(stock.cards.size()),temp};
        history.push_back(action);
        bin.cards.clear();
        
    }
    stock.arrange();
    bin.arrange();
}

inline bool klondike::flipCard(Slot* base){
    if (base->cards.size() != 0 &&
        base->cards.back()->faceUp == false){
        base->cards.back()->faceUp = true;
        return true;
    }
    return false;
}


bool klondike::validTableauMove(std::vector<Card*>& source, std::vector<Card*>& destination, Slot* base){
    if(destination.size()==0){
        if (source.front()->rank == 12){
            return true;
        }
        return false;
    }
    if(destination.back()->rank == source.front()->rank + 1 &&
        destination.back()->suit % 2 != source.front()->suit % 2){
            return true;
    }
    return false;
}

bool klondike::validFoundation(std::vector<Card*>& source, Foundation& foundation, Slot* base){

    if (source.size() == 1){
        Card* card = source[0];
        if(foundation.suit == -1){
            if(card->rank == 0){
                foundation.suit = card->suit;
                return true;
            }
            return false;
        }
        if(card->suit == foundation.suit){
            if(card->rank == foundation.getRank()){
                return true;
            }
        }
    }
    return false;
}

void klondike::movingCards(float mouseX, float mouseY){
    // Set the move for the cards.
    if (!move.draggedCards.empty()) {
        for (int i = 0; i < move.draggedCards.size(); i++){
            move.draggedCards[i]->rect.x = mouseX -move.offsetX;
            move.draggedCards[i]->rect.y = (mouseY - move.offsetY) + (i * move.gap);
        }
    }
}


void klondike::mouseDown(float mouseX,float mouseY){
    if(testCollision(mouseX,mouseY,stock.rect)){
        moveStock();
        //Move the cards in the stock.
        return;
    }

    //Check if the mouse press was on the bin.
    //If so take the card at the top.
    if(testCollision(mouseX,mouseY,bin.rect)){
        if (!bin.cards.empty()){
            move.draggedCards.push_back(bin.cards.back());
            bin.cards.pop_back();
            move.offsetX = mouseX - bin.rect.x;
            move.offsetY = mouseY - bin.rect.y;
            move.base = &bin;
        }
        return;
    }

// Now looking at the tableaus.
//Check the x axis to see if there is possible overlap.
//Check the y axis to see which card was clicked.
    for (Tableau &tab : tableau){
        bool overlap = 
        mouseX >= tab.rect.x && 
        mouseX <= tab.rect.x + dimensions::WIDTH;
        if (overlap){
            for (int i = tab.cards.size() - 1; i >= 0; --i){
                Card* myCard = tab.cards[i];
                if(myCard->faceUp==false) return;
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
            }
            tab.arrange();
        }
    }

}

void klondike::mouseUp(float mouseX, float mouseY){
    if (move.draggedCards.empty()) return;
        //Check if user clicked on foundation.
        for (Foundation &found : foundations){
            SDL_FRect box = found.rect;
            if(testCollision(mouseX,mouseY,box)){
                if(validFoundation(move.draggedCards,found,move.base)){
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
                if(validTableauMove(move.draggedCards,tab.cards,move.base)){
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

void klondike::Reset(){
    rng.seed(seed);
    clearGame();
    initKlondike();
}
void klondike::New(){
    startGame();
}


void klondike::startGame(){
    clearGame();
    seed = std::random_device{}();
    rng.seed(seed);
    initKlondike();
}
void klondike::clearGame(){
    deck.clear();
    tableau.clear();
    foundations.clear();
    move.draggedCards.clear();
    bin.cards.clear();
    stock.cards.clear();
    history.clear();
}

void klondike::drawGame(Rendering& renderer){
    renderer.drawBackground();
    for (const Slot &slot : tableau){
        renderer.drawRect(slot.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
        for (const Card* card : slot.cards){
            renderer.drawCardFace(*card);
        }
    }

    for (const Foundation &foundation :foundations){
        renderer.drawRect(foundation.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
        if (!foundation.cards.empty()){
            renderer.drawCardFace(*foundation.cards.back()); //only draw the last card.
        }
        
    }
    renderer.drawRect(stock.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
    renderer.drawRect(bin.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);


    for (const Card* card : stock.cards){
        renderer.drawCardFace(*card);
    }
    for (const Card* card:bin.cards){
        renderer.drawCardFace(*card);
    }

    for (const Card* card : move.draggedCards){
        renderer.drawCardFace(*card);
    }


}

void klondike::initKlondike(){
    static int count = 0;
    count++;

    deck.resize(klondikeNames::CARDCOUNT);
    tableau.resize(klondikeNames::TABLEAU);
    foundations.resize(klondikeNames::FOUNDATION);
    move.draggedCards.reserve(13);
    //Set up the card attributes.
    //Club = 0, Heart = 1, Spade = 2, Diamond = 3.
    for(int i = 0; i<klondikeNames::CARDCOUNT; i++){
        Card& card = deck[i];
        card.rank = (i % 13); // 0 -> 12
        card.suit = i / 13;
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


// Coordinates for the tableau. 
    for (int i = 0; i<klondikeNames::TABLEAU; i++){
        int Xpos = dimensions::TABX + i * (dimensions::WIDTH + dimensions::TABSPACING);
        tableau[i].rect.x = Xpos;
        tableau[i].rect.y = dimensions::TABY;
        tableau[i].rect.w = dimensions::WIDTH;
        tableau[i].rect.h = dimensions::HEIGHT;
    }
    
//foundations
    for (int i = 0; i<klondikeNames::FOUNDATION; i++){
        int Xpos = dimensions::FOUNDATIONX + i * (dimensions::WIDTH + dimensions::FOUNDATONSPACING);
        foundations[i].rect.x = Xpos;
        foundations[i].rect.y = dimensions::FOUNDATIONY;
        foundations[i].rect.w = dimensions::WIDTH;
        foundations[i].rect.h = dimensions::HEIGHT;
    }

// Set up the cards in the tableau.
    int curr = 0;
    for (int i = 0; i< klondikeNames::TABLEAU; i++){
        for (int j = 0; j <= i; j++){
            deck[curr].rect.x = tableau[i].rect.x;
            deck[curr].rect.y = tableau[i].rect.y + (j * dimensions::gap(i+1));
            tableau[i].cards.push_back(&deck[curr]);
            curr += 1;
        }
        tableau[i].cards.back()->faceUp = true;
    }

    stock.rect.x = dimensions::STOCKX;
    stock.rect.y = dimensions::STOCKY;
    stock.rect.w = dimensions::WIDTH;
    stock.rect.h = dimensions::HEIGHT;

    for (curr; curr < klondikeNames::CARDCOUNT; curr++){
        deck[curr].rect.x = stock.rect.x;
        deck[curr].rect.y = stock.rect.y;
        stock.cards.push_back(&deck[curr]);
    }
    
    bin.rect.x = dimensions::STOCKX + dimensions::STOCKSPACING + dimensions::WIDTH;
    bin.rect.y = dimensions::STOCKY;
    bin.rect.w = dimensions::WIDTH;
    bin.rect.h = dimensions::HEIGHT;
}
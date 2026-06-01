#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include "FreeCellData.h"
#include "FreeCell.h"
#include "UndoAction.h"


freeCell::freeCell(){startGame();}

void freeCell::moveCards(std::vector<Card*>& source, std::vector<Card*>& destination){
    destination.insert(destination.end(),source.begin(),source.end());
    source.clear();
}

bool freeCell::testWin(){
    for(const Stock &cell: cells){
        if(!cell.cards.empty()) return false;
    }
    for(const Tableau &tab: tableau){
        if(!tab.cards.empty()) return false;
    }
    return false;
}

void freeCell::update(float mouseX, float mouseY,float time){
    movingCards(mouseX, mouseY);
}



void freeCell::undo(){

    if(history.size() == 0) return;
    UndoAction action = history.back(); 

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


bool freeCell::testCollision(float mouseX, float mouseY, SDL_FRect& box){
    if (mouseX >=box.x &&
        mouseX <= box.x + box.w &&
        mouseY >= box.y &&
        mouseY <= box.y + box.h){
            return true;
    }
    return false;
}


void freeCell::shuffle(std::vector<Card>& cards) {
    for (int i = 0; i < cards.size(); i++) {
        std::uniform_int_distribution<int> dist(i, cards.size() - 1);
        int j = dist(rng);
        std::swap(cards[i], cards[j]);
    }
}


// If empty, any card can move. Otherwise, use the standard klondike code.
bool freeCell::validTableauMove(std::vector<Card*>& source, std::vector<Card*>& destination){
    if(destination.size()==0) return true;

    if(destination.back()->rank == source.front()->rank + 1 &&
        destination.back()->suit % 2 != source.front()->suit % 2){
            return true;
    }
    return false;
}



bool freeCell::validFoundation(std::vector<Card*>& source, Foundation& foundation){
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


void freeCell::movingCards(float mouseX, float mouseY){
    // Set the move for the cards.
    if (!move.draggedCards.empty()) {
        for (int i = 0; i < move.draggedCards.size(); i++){
            move.draggedCards[i]->rect.x = mouseX -move.offsetX;
            move.draggedCards[i]->rect.y = (mouseY - move.offsetY) + (i * move.gap);
        }
    }
}


void freeCell::mouseDown(float mouseX,float mouseY){
    for(Stock &cell: cells){
        SDL_FRect box = cell.rect;
        if(testCollision(mouseX,mouseY,box)){
            if (cell.cards.empty()) return;
            Card* myCard = cell.cards.back();
            move.draggedCards.push_back(myCard);
            move.offsetX = mouseX - myCard->rect.x;
            move.offsetY = mouseY - myCard->rect.y;
            move.base = &cell;
            move.gap = dimensions::gap(move.draggedCards.size());
            cell.cards.pop_back();
            return;
        }
    }

// Now looking at the tableaus.
//Check the x axis to see if there is possible overlap between tableaus first.
//Then check the y axis to see which card was clicked.
    for (Tableau &tab : tableau){
        bool overlap = 
        mouseX >= tab.rect.x && 
        mouseX <= tab.rect.x + dimensions::WIDTH;
        if (overlap){
            int colour = (tab.cards.back()->suit+1) % 2;
            int rank = tab.cards.back()->rank - 1;
            for (int i = tab.cards.size() - 1; i >= 0; --i){
                Card* myCard = tab.cards[i];
                if(myCard->suit %2 == colour || myCard->rank !=rank+1) {
                    return;
                }
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
                colour = myCard->suit;

            }
            tab.arrange();
        }
    }
}


void freeCell::mouseUp(float mouseX, float mouseY){
    if (move.draggedCards.empty()) return;
        for (Slot &cell : cells){
            SDL_FRect box = cell.rect;
            if(testCollision(mouseX,mouseY,box)){
                if(cell.cards.empty() && move.draggedCards.size() == 1){
                    moveCards(move.draggedCards,cell.cards);
                    UndoAction action = {UndoType::moveCards,move.base,&cell,1,{},true};
                    history.push_back(action);
                    cell.arrange();
                    return;
                }
            }
        }

        for (Foundation &found : foundations){
            SDL_FRect box = found.rect;
            if(testCollision(mouseX,mouseY,box)){
                if(validFoundation(move.draggedCards,found)){
                    int size = static_cast<int>(move.draggedCards.size());
                    moveCards(move.draggedCards,found.cards);
                    UndoAction action = {UndoType::moveCards,move.base,&found,size,{},true};
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
                    UndoAction action = {UndoType::moveCards,move.base,&tab,size,{},true};
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

void freeCell::Reset(){
    rng.seed(seed);
    clearGame();
    initFreeCell();
}
void freeCell::New(){
    startGame();
}


void freeCell::startGame(){
    clearGame();
    seed = std::random_device{}();
    rng.seed(seed);
    initFreeCell();
}
void freeCell::clearGame(){
    deck.clear();
    tableau.clear();
    foundations.clear();
    move.draggedCards.clear();
    history.clear();
}

void freeCell::drawGame(Rendering& renderer){
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

    for (const Stock &cell : cells){
        renderer.drawRect(cell.rect,{0,255,0,SDL_ALPHA_OPAQUE},false);
        for(const Card* card: cell.cards){
            renderer.drawCardFace(*card);
        }
    }


    for (const Card* card : move.draggedCards){
        renderer.drawCardFace(*card);
    }

}

void freeCell::initFreeCell(){
    static int count = 0;
    count++;

    deck.resize(FreeCellData::CARDCOUNT);
    tableau.resize(FreeCellData::TABLEAU);
    foundations.resize(FreeCellData::FOUNDATION);
    cells.resize(FreeCellData::CELL);

    move.draggedCards.reserve(13);
    //Set up the card attributes.
    //Club = 0, Heart = 1, Spade = 2, Diamond = 3.
    for(int i = 0; i<FreeCellData::CARDCOUNT; i++){
        Card& card = deck[i];
        card.rank = (i % 13); // 0 -> 12
        card.suit = i / 13;
        card.rect.w = FreeCellData::WIDTH;
        card.rect.h = FreeCellData::HEIGHT;
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
    for (int i = 0; i<FreeCellData::TABLEAU; i++){
        int Xpos = FreeCellData::TABX + i * (FreeCellData::WIDTH + FreeCellData::SPACING);
        tableau[i].rect.x = Xpos;
        tableau[i].rect.y = FreeCellData::TABY;
        tableau[i].rect.w = FreeCellData::WIDTH;
        tableau[i].rect.h = FreeCellData::HEIGHT;
    }
    
//foundations
    for (int i = 0; i<FreeCellData::FOUNDATION; i++){
        int Xpos = FreeCellData::FOUNDATIONX + i * (FreeCellData::WIDTH + FreeCellData::SPACING);
        foundations[i].rect.x = Xpos;
        foundations[i].rect.y = FreeCellData::FOUNDATIONY;
        foundations[i].rect.w = FreeCellData::WIDTH;
        foundations[i].rect.h = FreeCellData::HEIGHT;
    }

//Cell
    for (int i = 0; i<FreeCellData::CELL; i++){
        int Xpos = FreeCellData::CELLX + i * (FreeCellData::WIDTH + FreeCellData::SPACING);
        cells[i].rect.x = Xpos;
        cells[i].rect.y = FreeCellData::CELLY;
        cells[i].rect.w = FreeCellData::WIDTH;
        cells[i].rect.h = FreeCellData::HEIGHT;
    }


// Set up the cards in the tableau.
    int curr = 0;
    for (int i = 0; i< 4; i++){
        for (int j = 0; j < 7; j++){
            deck[curr].rect.x = tableau[i].rect.x;
            deck[curr].rect.y = tableau[i].rect.y + (j * FreeCellData::gap(j+1));
            tableau[i].cards.push_back(&deck[curr]);
            deck[curr].faceUp = true;
            curr += 1;
        }
    }
    for (int i = 4; i< 8; i++){
        for (int j = 0; j < 6; j++){
            deck[curr].rect.x = tableau[i].rect.x;
            deck[curr].rect.y = tableau[i].rect.y + (j * FreeCellData::gap(j+1));
            tableau[i].cards.push_back(&deck[curr]);
            deck[curr].faceUp = true;
            curr += 1;
        }
    }
    
}
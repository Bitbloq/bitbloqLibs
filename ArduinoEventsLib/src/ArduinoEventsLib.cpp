//#define fakearduino

#include "ArduinoEventsLib.h"
#include <Arduino.h>

ActionItem::ActionItem(functionPointer func, unsigned long t):
    timestamp{t},
    f{func},
    next{nullptr},
    prev{nullptr}
{

}

Heap::Heap():
    first{nullptr},
    last{nullptr},
    size{0}
{

}

Heap::~Heap(){

    ActionItem* it = first;
    while(it!=nullptr){
        ActionItem* aux = it->next;
        delete it;
        it=aux;
    }
    first = nullptr;
    last = nullptr;
}


void Heap::insert(functionPointer p, int delay){
    Serial.print("Insert with delay: ");
    Serial.println(millis() + delay);
    ActionItem* cb = new ActionItem(p, millis() + delay);
    if(last){
        //introduce the new one after last
        last->next = cb;
        cb->prev = last;
        last = cb;
    }else{
        //it is the first one
        first = cb;
        last = cb;
    }

    size++;
    Serial.print("Insert - size: "); Serial.println(size);
}

void Heap::remove(ActionItem *cb){
    Serial.print("Trying to remove - size: "); Serial.println(size);
    //If cb is the first iteam in the heap
    if(cb == first){
        // the only  one
        if(cb == last){
            first = nullptr;
            last = nullptr;
        }else{
            //if cb is NOT de last item in the heap
            first = cb->next;
            first->prev = nullptr;
        }
    }else if (cb == last){
        last = cb->prev;
        last->next = nullptr;
    }else{
        //if cb is NOT the first, nor the last item in the heap
        cb->prev->next = cb->next;
        cb->next->prev = cb->prev;
    }
    size--;
    delete cb;
    Serial.print("Removed - size: "); Serial.println(size);

}

void Heap::eventloop(){
    ActionItem* it = first;
    while(it!=nullptr){
        ActionItem* aux = it;
        it = it->next;
        if(aux->timestamp < millis()){
            aux->f();
            remove(aux);
        }
    }
}

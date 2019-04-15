//#define fakearduino

#ifndef ARDUINOEVENTSLIB_H
#define ARDUINOEVENTSLIB_H


typedef void (*functionPointer)(void);

class ActionItem
{
public:
    ActionItem(functionPointer func, unsigned long t);
    unsigned long timestamp;
    functionPointer f;
    ActionItem* next;
    ActionItem* prev;
};

class Heap
{
public:
    Heap();
    ~Heap();
    ActionItem* first;
    ActionItem* last;
    void insert(functionPointer, int delay = 0);
    void remove(ActionItem *);
    void eventloop();
    int size;
};

#endif // ARDUINOEVENTSLIB_H

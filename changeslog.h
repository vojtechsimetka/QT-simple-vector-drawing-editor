#ifndef CHANGESLOG_H
#define CHANGESLOG_H

#include "data.h"

// Actions which could be done
enum Actions {
    ADD,
    DELETE,
    MOVE
};

// Objects acting in changes
enum Elements {
    LINE,
    RECTANGLE
};

// One step possible to do in app
struct Step {
    Actions action;
    Elements element;
    int offsetX;
    int offsetY;
    void *object;
};

class ChangesLog
{
private:
    // Private constructor
    static ChangesLog *changesLogInstance;
    ChangesLog();

    // Connection to data
    Data data;
    // Array of changes
    Step undoList[10];
    // One step behind last item
    int positionInUndoList;
    //Count items of changes
    int itemsInUndoArray;

    // Array of undo changes
    Step redoList[10];
    // One step behind last item
    int positionInRedoList;
    // Count items of undos
    int itemsInRedoArray;

public:
    static ChangesLog *sharedInstance();



    void init(Data d);
};

#endif // CHANGESLOG_H

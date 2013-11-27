#ifndef CHANGESLOG_H
#define CHANGESLOG_H

#include "data.h"

// Actions which could be done
enum Actions {
    ADD,
    DELETE,
    MOVE
};

// One step possible to do in app
struct Step {
    Actions action;
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

    // List of changes
    QLinkedList<Step*> changes;
    Step *currentItem;

    // Connection to data
    Data *data;

public:
    static ChangesLog *sharedInstance();
    void init(Data *d);
    void doStep(Actions a, int offsetX, int offsetY, void *object);
    void undoStep();
    void redoStep();
};

#endif // CHANGESLOG_H

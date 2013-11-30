/**
 * PGR 2013 project
 * Used for tracking changes for redo and undo buttons
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    changeslog.h
 */
#ifndef CHANGESLOG_H
#define CHANGESLOG_H

#include "data.h"

// Actions that can be done
enum Actions
{
    ADD,
    DELETE,
    MOVE
};

// Steps possible to do in app
struct Step
{
    Actions action;
    int offsetX;
    int offsetY;
    void *object;
};

class ChangesLog
{
private:
    static ChangesLog *changesLogInstance;
    ~ChangesLog();
    std::vector<Step *> changes;
    // Pointer behind last change in changes
    int lastChange;
    Data *data;

public:
    static ChangesLog *sharedInstance();
    void init(Data *d);
    void doStep(Actions a, int offsetX, int offsetY, void *object);
    void undoStep();
    void redoStep();
};

#endif // CHANGESLOG_H

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
#include "point.h"
#include "selection.h"

// Actions that can be done
enum Actions
{
    ADD,
    DELETE,
    MOVE,
    ROTATION,
    RESIZE
};

// Steps possible to do in app
struct Step
{
    Actions action;
    int offsetX;
    int offsetY;
    Point *origin;
    Point *changed;
    Point *cor;
    float angle;
    Qt::Corner orientation;
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
    Selection *selection;

public:
    static ChangesLog *sharedInstance();
    void init(Data *d, Selection *selection);
    void doStep(Actions a, int offsetX, int offsetY, void *object);
    void doStep(Actions a, Point *min, Point *max, Qt::Corner orientation, void *object);
    void doStep(Actions a, Point *cor, float angle, void *object);
    void undoStep();
    bool canUndo();
    void redoStep();
    bool canRedo();
};

#endif // CHANGESLOG_H

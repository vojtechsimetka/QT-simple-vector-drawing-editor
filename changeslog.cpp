/**
 * PGR 2013 project
 * Used for tracking changes for redo and undo buttons
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    changeslog.cpp
 */
#include "changeslog.h"
#include "mainwindow.h"

ChangesLog *ChangesLog::changesLogInstance = new ChangesLog();

/**
 * @brief ChangesLog destructor
 */
ChangesLog::~ChangesLog()
{
    // TODO: Dealocate everything
}

/**
 * @brief ChangesLog singleton constructor
 * @return Singleton reference
 */
ChangesLog *ChangesLog::sharedInstance()
{
    return changesLogInstance;
}

/**
 * @brief Initializes changeslog
 * @param d Data reference
 */
void ChangesLog::init(Data *d, Selection *selection)
{
    this->data = d;
    this->selection = selection;
    this->lastChange = 0;
}

/**
 * @brief Log one step
 * @param a Action to log
 * @param offsetX X offset if object was moved.
 * @param offsetY Y offset if object was moved.
 * @param object Object ID
 */
void ChangesLog::doStep(Actions a, int offsetX, int offsetY, void *object)
{
    // Remove all possible redo
    while ((int)changes.size() != lastChange)
        changes.pop_back();

    // Create new step
    Step *s = new Step();
    s->action = a;
    s->offsetX = offsetX;
    s->offsetY = offsetY;
    s->object = object;

    // Add step to list of changes
    changes.push_back(s);
    lastChange++;
}

/**
 * @brief Log one step
 * @param a Action to log
 * @param offsetX X offset if object was moved.
 * @param offsetY Y offset if object was moved.
 * @param object Object ID
 */
void ChangesLog::doStep(Actions a, Point *origin, Point *changed, Qt::Corner orientation, void *object)
{
    // Remove all possible redo
    while ((int)changes.size() != lastChange)
        changes.pop_back();

    // Create new step
    Step *s = new Step();
    s->action = a;
    s->origin = origin;
    s->changed = changed;
    s->orientation = orientation;
    s->object = object;

    // Add step to list of changes
    changes.push_back(s);
    lastChange++;
}

/**
 * @brief Undo step
 */
void ChangesLog::undoStep()
{
    // Nothing to undo
    if (!this->canUndo())
        return;

    // Point to last change
    lastChange--;

    std::vector<Element *> *list;

    // Undo action
    Step *s = (Step *)changes.at(lastChange);
    switch (s->action)
    {
    case ADD:
        data->remove((Element*)s->object);
        break;
    case DELETE:
        list = (std::vector<Element *> *) s->object;
        foreach (Element *e, *list) {
            data->add(e);
        }
        break;
    case MOVE:
        list = (std::vector<Element *> *) s->object;
        foreach (Element *e, *list) {
            e->translatef(-s->offsetX, -s->offsetY);
        }
        this->selection->calculateBoundingRectangle();
        break;
    case RESIZE:
        list = (std::vector<Element *> *) s->object;
        //this->selection->deselectAll();
        foreach (Element *e, *list) {
            this->selection->addBack(e);
        }
        this->selection->calculateBoundingRectangle();
        this->selection->storeDistancesToFixedPoint();
        this->selection->resizeSelectedItems(s->origin->getX(),
                                             s->origin->getY(),
                                             s->changed->getX(),
                                             s->changed->getY(),
                                             s->orientation);
        break;
    default:
        break;
    }
}

bool ChangesLog::canUndo()
{
    if (changes.empty() || lastChange == 0)
        return false;

    return true;
}

/**
 * @brief Redo step
 */
void ChangesLog::redoStep()
{
    // Nothing to redo
    if (!this->canRedo())
        return;

    std::vector<Element *> *list;

    // Undo action
    Step *s = (Step *)changes.at(lastChange);
    switch (s->action)
    {
    case ADD:
        data->add((Element*)s->object);
        break;
    case DELETE:
        list = (std::vector<Element *> *) s->object;
        foreach (Element *e, *list) {
            data->remove(e);
        }
        break;
    case MOVE:
        list = (std::vector<Element *> *) s->object;
        foreach (Element *e, *list) {
            e->translatef(s->offsetX, s->offsetY);
        }
        this->selection->calculateBoundingRectangle();
        break;
    default:
        break;
    }

    lastChange++;
}

bool ChangesLog::canRedo()
{
    if (changes.empty() || ((int)changes.size() == lastChange))
        return false;

    return true;
}

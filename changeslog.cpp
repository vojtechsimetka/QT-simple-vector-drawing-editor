#include "changeslog.h"
#include "mainwindow.h"

ChangesLog *ChangesLog::changesLogInstance = new ChangesLog();

ChangesLog::ChangesLog()
{

}

ChangesLog *ChangesLog::sharedInstance()
{
    return changesLogInstance;
}

void ChangesLog::init(Data *d)
{
    data = d;
    lastChange = 0;
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
    // TODO: Remove when logging is in opengl widget
    // Check if do is redo step
    if ((int)changes.size() != lastChange)
    {
        Step *s = (Step *)changes.at(lastChange);
        // Return, step is in the list
        if (s->object == object)
            return;
    }

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
 * @brief Undo step
 */
void ChangesLog::undoStep()
{
    // Nothing to undo
    if (changes.empty() || lastChange == 0)
        return;

    // Point to last change
    lastChange--;

    // Undo action
    Step *s = (Step *)changes.at(lastChange);
    switch (s->action)
    {
    case ADD:
        data->remove((Element*)s->object);
        break;
    case DELETE:
        data->add((Element*)s->object);
        break;
    default:
        break;
    }

    // Repaint glWindow
    MainWindow::opw->repaint();
}

/**
 * @brief Redo step
 */
void ChangesLog::redoStep()
{
    // Nothing to redo
    if (changes.empty() || ((int)changes.size() == lastChange))
        return;

    // Undo action
    Step *s = (Step *)changes.at(lastChange);
    switch (s->action)
    {
    case ADD:
        data->add((Element*)s->object);
        break;
    case DELETE:
        data->remove((Element*)s->object);
        break;
    default:
        break;
    }

    lastChange++;

    // Repaint glWindow
    MainWindow::opw->repaint();
}

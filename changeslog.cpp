#include "changeslog.h"

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
    changesLogInstance->data = d;
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
    // Remove all possible action for REDO
    while ((!changes.isEmpty()) && (!changes.endsWith(currentItem)))
        changes.removeLast();

    // Create new step
    Step *s = new Step();
    s->action = a;
    s->offsetX = offsetX;
    s->offsetY = offsetY;
    s->object = object;

    // Add step to list of changes
    changes.append(s);
    currentItem = s;

    qDebug() << "add,curr: " << currentItem;
    qDebug() << "add,last: " <<changes.last();
}

/**
 * @brief Undo step
 */
void ChangesLog::undoStep()
{
    // Nothing for undo
    if (changes.isEmpty() || currentItem == NULL)
        return;

    // Undo action
    switch (currentItem->action)
    {
    case ADD:
        data->remove((Element *)currentItem->object);
        // TODO: Delete? Element has no virtual destructor..
        break;
    case DELETE:
        data->add((Element *)currentItem->object);
        break;
    default:
        break;
    }

    // Update changes list
    if (currentItem == changes.first())
    {
        currentItem = NULL;
    }
    else
    {
        QLinkedListIterator<Step *> it(changes);
        while (it.next() != currentItem) { }
        currentItem = it.previous();
    }



    qDebug() << "red,curr: " << currentItem;
    qDebug() << "red,last: " <<changes.last();
}

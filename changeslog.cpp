#include "changeslog.h"

ChangesLog *ChangesLog::changesLogInstance = NULL;

ChangesLog::ChangesLog()
{

}

ChangesLog *ChangesLog::sharedInstance()
{
    if (changesLogInstance == NULL)
    {
        changesLogInstance = new ChangesLog();
    }

    return changesLogInstance;
}

void ChangesLog::init(Data d)
{
    this->data = d;
}

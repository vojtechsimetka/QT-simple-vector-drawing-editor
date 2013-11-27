#include "data.h"
#include "changeslog.h"
#include <QDebug>

Data::Data()
{
    this->createLine(100, 100, 300, 300);
    this->createLine(200, 150, 500, 600);
}

void Data::createLine(float x1, float y1, float x2, float y2)
{
    this->elements.append(new Line(x1, y1, x2, y2));
    ChangesLog::sharedInstance();
}

void Data::createRectangle(float x1, float y1, float x2, float y2)
{
    this->elements.append(new Rectangle(x1, y1, x2, y2));
}

void Data::paintAll()
{
    foreach (Element* e, this->elements)
        e->PaintMe();
}

void Data::add(Element * e)
{
    this->elements.append(e);
}

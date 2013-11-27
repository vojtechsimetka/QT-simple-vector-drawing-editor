#include "data.h"
#include "changeslog.h"
#include <QDebug>

Data::Data()
{
    this->createLine(80, 100, 300, 300);
    this->createLine(100, 100, 300, 300);
    this->createLine(200, 150, 500, 600);
    ChangesLog::sharedInstance()->init(this);
}

void Data::createLine(float x1, float y1, float x2, float y2)
{
    Line *l = new Line(x1, y1, x2, y2);
    this->elements.append(l);
    ChangesLog::sharedInstance()->doStep(ADD,0,0,l);
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
    ChangesLog::sharedInstance()->doStep(ADD,0,0,e);
}

void Data::remove(Element *e)
{
    this->elements.removeOne(e);
}

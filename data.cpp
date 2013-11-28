#include "data.h"
#include "changeslog.h"
#include <QDebug>

Data::Data()
{
    ChangesLog::sharedInstance()->init(this);
}

void Data::createLine(float x1, float y1, float x2, float y2)
{
    Line *l = new Line(x1, y1, x2, y2);
    this->elements.push_back(l);
    ChangesLog::sharedInstance()->doStep(ADD,0,0,l);
}

void Data::createRectangle(float x1, float y1, float x2, float y2)
{
    this->elements.push_back(new Rectangle(x1, y1, x2, y2));
}

void Data::paintAll()
{
    foreach (Element* e, this->elements)
        e->PaintMe();
}

void Data::add(Element * e)
{
    this->elements.push_back(e);
    // TODO: Log at openglwidget  !!!
    ChangesLog::sharedInstance()->doStep(ADD,0,0,e);
}

/**
 * @brief Remove specified element
 * @param e Element to remove
 */
void Data::remove(Element *e)
{
    // Erase element
    std::vector<Element *>::iterator it = std::find(this->elements.begin(), this->elements.end(), e);
    if(it != this->elements.end())
        this->elements.erase(it);
    // TODO: Log at openglwidget  !!!
    ChangesLog::sharedInstance()->doStep(DELETE,0,0,e);
}

/**
 * @brief Dehighlight all elemets
 */
void Data::deHighlightAll()
{
    foreach (Element *e, this->elements) {
        e->deHighlightMe();
    }
}

std::vector<Element *> Data::getElements()
{
    return elements;
}


/**
 * PGR 2013 project
 * MetaElement header file
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    metaelement.h
 */
#ifndef METAELEMENT_H
#define METAELEMENT_H

#include "element.h"
#include "point.h"

class MetaElement
{
public:
    MetaElement();
    ~MetaElement();
    bool isEmpty();
    void init(Element *element, float x, float y);
    void paintMe();
    void highlightMe();
    void deHighlightMe();
    void clear();
    void resizeTo(float x, float y);
    Element *getElement();

    Point getOrigin() const;

private:
    Element *element;
    Point origin;
};

#endif // METAELEMENT_H

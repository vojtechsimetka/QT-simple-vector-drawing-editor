/**
 * PGR 2013 project
 * Abstract class used for all objects that can be drawn
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    element.h
 */
#ifndef ELEMENT_H
#define ELEMENT_H

#include "point.h"

class Element
{
public:
    Element();
    virtual void paintMe() const = 0;
    virtual void paintPoints() const = 0;
    virtual void resize(float x1, float y1, float x2, float y2) = 0;
    virtual bool intersects(float min_x, float min_y, float max_x, float max_y) const = 0;
    virtual bool intersects(Point) const = 0;
    void highlightMe();
    void deHighlightMe();
    void selectMe();
    void deSelectMe();

protected:
    bool highlighted;
    bool selected;
};

#endif // ELEMENT_H

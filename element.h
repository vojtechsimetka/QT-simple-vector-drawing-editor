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

namespace ElementType
{
    typedef enum
    {
        POLYGON,
        CIRCLE,
        LINE,
        SELECTION_RECTANGLE //NEVER use in drawing
    } Type;
}

class Element
{
public:
    Element(ElementType::Type type);
    virtual ~Element();
    virtual void paintMe() const = 0;
    virtual void paintPoints() const = 0;
    virtual void resize(float x1, float y1, float x2, float y2) = 0;
    virtual bool intersects(float min_x, float min_y, float max_x, float max_y) const = 0;
    virtual bool intersects(Point) const = 0;
    virtual bool intersects(float, float) const = 0;
    virtual bool getCounterPoint(float, float, float *, float *) const = 0;
    virtual float getMinX() const = 0;
    virtual float getMinY() const = 0;
    virtual float getMaxX() const = 0;
    virtual float getMaxY() const = 0;
    virtual void  resizeToBoundingRectangle(float, float, float, float) = 0;
    virtual void finalizeResize() = 0;
    virtual void translatef(float, float) = 0;
    void highlightMe();
    void deHighlightMe();
    void selectMe();
    void deSelectMe();
    bool isSelected() const;
    ElementType::Type getType() const;
    void setOffset(float, float);

protected:
    bool highlighted;
    bool selected;
    const ElementType::Type type;
};

#endif // ELEMENT_H

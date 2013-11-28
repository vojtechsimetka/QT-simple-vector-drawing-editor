#ifndef LINE_H
#define LINE_H

#include "element.h"
#include "point.h"
#include "openglwidget.h"

class Element;

class Line : public Element
{
public:
    Line(float x1, float y1, float x2, float y2);
    ~Line();
    void PaintMe() const;
    void PaintPoints() const;
    void resize(float x1, float y1, float x2, float y2);

    Point getP1() const;
    void setP1(float x, float y);

    Point getP2() const;
    void setP2(float x, float y);

private:
    Point p1;
    Point p2;
    float red;
    float green;
    float blue;
};

#endif // LINE_H

/**
 * PGR 2013 project
 * Line representation in editor
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    line.cpp
 */
#ifndef LINE_H
#define LINE_H

#include "element.h"
#include "point.h"
#include <QGLWidget>
#include <QtOpenGL>

class Element;

class Line : public Element
{
public:
    Line(float x1, float y1, float x2, float y2);
    ~Line();
    virtual void paintMe() const;
    void paintPoints() const;
    void resize(float x1, float y1, float x2, float y2);
    bool intersects(SelectionRectangle, Point offset);

    Point getP1() const;
    void setP1(float x, float y);

    Point getP2() const;
    void setP2(float x, float y);

private:
    Point p1;
    Point p2;

protected:
    float red;
    float green;
    float blue;
};

#endif // LINE_H

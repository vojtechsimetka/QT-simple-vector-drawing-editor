/**
 * PGR 2013 project
 * Selection rectangle implementation
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/29
 * @version 1
 * @file    selectionrectangle.h
 */
#ifndef SELECTIONRECTANGLE_H
#define SELECTIONRECTANGLE_H

#include <QGLWidget>
#include <QtOpenGL>
#include "point.h"
#include "element.h"
#include "line.h"

class Element;

class SelectionRectangle
{
public:
    SelectionRectangle();
    void paintMe();
    void resize(float, float, float, float, const Point offset, float scale);
    void deactivate();
    void transform(Point offset, float scale);
    bool intersects(Point point);
    bool intersects(float x, float y);
    bool intersects(const Line *l);
    bool intersects(const Element *l);
    bool isActive() const;

private:
    float min_x;
    float min_y;
    float max_x;
    float max_y;
    float width;
    float height;
    bool active;

    float translated_min_x;
    float translated_min_y;
    float translated_max_x;
    float translated_max_y;
};

#endif // SELECTIONRECTANGLE_H

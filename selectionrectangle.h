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

class SelectionRectangle
{
public:
    SelectionRectangle();
    void paintMe();
    void resize(float, float, float, float);
    void deactivate();
    bool intersects(Point point, Point offset);
    bool intersects(float x, float y, Point offset);

private:
    float min_x;
    float min_y;
    float max_x;
    float max_y;
    float width;
    float height;
    bool active;
};

#endif // SELECTIONRECTANGLE_H

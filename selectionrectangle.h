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
#include "rectangle.h"
#include "line.h"
#include "gui.h"

class Element;
class Gui;
class Rectangle;

class SelectionRectangle : public Element
{
public:
    SelectionRectangle(Gui * gui);
    ~SelectionRectangle();
    void paintMe() const;
    void paintPoints() const;
    void resize(float x1, float y1, float x2, float y2);
    void resize(float x1, float y1, float x2, float y2, Qt::Corner orientation);
    bool intersects(float min_x, float min_y, float max_x, float max_y) const;
    bool intersects(Point) const;
    bool intersects(float, float) const;
    bool getCounterPoint(float, float, float*, float*) const;
    float getMinX() const;
    float getMinY() const;
    float getMaxX() const;
    float getMaxY() const;
    void  resizeToBoundingRectangle(float, float, float, float);
    void translatef(float, float);

    void paintBoundingRectangle(float, float) const;
    void resize(float, float, float, float, const Point offset, float scale);
    bool getCounterPointAndCalculatePoints(float x, float y, float *ox, float *oy);
    void deactivate();
    void transform(Point offset, float scale);
    bool selectionIntersects(Point point) const;
    bool selectionIntersects(float x, float y) const;
    bool selectionIntersects(const Line *l) const;
    bool selectionIntersects(const Element *l) const;
    bool isActive() const;
    bool empty() const;
    Element * front() const;
    std::vector<Element *> getSelectedItems() const;
    void clear();
    void addBack(Element *e);
    bool contains(Element *e) const;
    void finalizeResize();

    void startDragging(float x, float y);
    bool isDragged() const;
    void finishDragging(float, float);
    void drag(float, float);

    void calculateBoundingRectangle();
    void storeDistancesToFixedPoint();

    Qt::Corner getOrientation();

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

    std::vector<Element *> selected_items;
    Gui *gui;
    Rectangle *bounding_rectangle;
    bool valid_bounding_rectangle;

    void deselectAll();

    std::vector<float> list_of_points;
    Qt::Corner orientation;
    float minx;
    float miny;
    float maxx;
    float maxy;

    bool dragging_items;
    float start_x;
    float start_y;
    float offset_x;
    float offset_y;
};

#endif // SELECTIONRECTANGLE_H

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

class Selection
{
public:
    Selection(Gui * gui);
    ~Selection();
    void paintMe() const;
    void paintBoundingRectangle(float, float) const;

    void resizeSelectionRectangle(float, float, float, float, const Point offset, float scale);
    bool isResizePointClicked(float x, float y);
    void deactivate();
    void transform(Point offset, float scale);
    bool selectionIntersects(Point point) const;
    bool selectionIntersects(float x, float y) const;
    bool selectionIntersects(const Line *l) const;
    bool selectionIntersects(const Element *l) const;
    bool isActive() const;
    bool empty() const;
    Element *front() const;
    std::vector<Element *> getSelectedItems() const;
    void clear();
    void addBack(Element *e);
    bool contains(Element *e) const;
    void remove(Element *e);

    // Resizing
    void finalizeResize();
    void startResize();
    bool isResized();
    void resizeSelectedItems(float x1, float y1, float x2, float y2, Qt::Corner orientation);
    void resizeSelectedItems(float x2, float y2);
    void calculateBoundingRectangle();
    void storeDistancesToFixedPoint();

    // Dragging
    void startDragging(float x, float y);
    bool isDragged() const;
    void finishDragging(float, float);
    void drag(float, float);

    // Rotation
    bool isCentreOfRotation(float, float);
    bool isRotationPoint(float, float);
    bool isCentreOfRotationDragged();
    bool isRotationPointDragged();
    void dragCentreOfRotation(float, float);
    void dragRotationPoint(float, float);
    void setCentreOfRotationDragged(bool);
    void setRotationPointDragged(bool);
    void rotate(Point, float);

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
    bool resized;
    Point origin;

    bool dragging_items;
    float start_x;
    float start_y;
    float offset_x;
    float offset_y;

    Point centre_of_rotation;
    Point rotation_point;
    bool centre_of_origin_dragged;
    bool rotation_point_dragged;
    float rotation;
};

#endif // SELECTIONRECTANGLE_H

/**
 * PGR 2013 project
 * OpenGL widget class header
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    openglwidget.h
 */
#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include "data.h"
#include "metaelement.h"
#include "guideline.h"
#include "selectionrectangle.h"

const int MINDISTANCE = 8;
const int INF = 10000;

// Necesarry declarations for linker
class Data;
class Line;
class GuideLine;

// Drawer state enumerator
typedef enum
{
    DRAW,
    SELECT_E,
    ROTATE,
    DLT,
    CHANGESIZE,
    PAN
} Status;

// Catch status enumerator
typedef enum
{
    CLASSIC,
    PARALLEL,
    PERPENDICULAR,
    MIDDLE
} CatchStatus;

class OpenGLWidget : public QGLWidget
{
public:
    OpenGLWidget(QWidget *parent = NULL);
    ~OpenGLWidget();
    void initializeGL();
    void paintGL();
    void setAction(Status s);
    void setAction(ElementType::Type t);
    void deleteSelection();
    void changeLength(float length);
    Element *topObjectAtMousePosition();
    static float treshold_value;

private:
    Data *data;
    Status status;
    ElementType::Type type;
    CatchStatus catchStatus;
    MetaElement metaElement;
    GuideLine *vertical_guideline;
    GuideLine *horizontal_guideline;
    Point offset;
    Point aux_offset;
    Point mouse_start_position;
    Point mouse_end_position;
    float scale;
    SelectionRectangle selection_rectangle;
    std::vector<Element *> selected_items;
    bool items_being_dragged;

    bool isHorizontal(float y1, float y2);
    bool isVertical(float x1, float x2);
    bool catchToParallelLine(float x11, float y11, float *x21, float *y21);
    bool catchToPerpendicular(float x11, float y11, float *x21, float *y21);
    bool catchToDiagonal(float *x1, float *y1, float x2, float y2);
    void catchToClosePoint(float *x, float *y);
    void catchToMiddleOfLine(float *x, float *y);
    void mouseReleaseDraw(float x, float y);
    void createNewElement(float x, float y);
    QString qKeyEventToQString(QKeyEvent *keyEvent);
    void mouseReleaseSelect();
    void mousePressSelect();
    void mouseMoveDraw(float *x, float *y);
    float translateX(float);
    float translateY(float);
    bool selectionContains(Element *);

protected:
    void resizeGL(int w, int h);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *keyEvent);
    void wheelEvent(QWheelEvent* event);
};

#endif // OPENGLWIDGET_H

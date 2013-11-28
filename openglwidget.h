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

#define MINDISTANCE 8

#include <QGLWidget>
#include <QtOpenGL>
#include "data.h"
#include "metaelement.h"
#include "line.h"

// Necesarry declarations for linker
class Data;
class Line;

// Drawer state enumerator
typedef enum
{
    DRAWLINE,
    DRAWRECTANGLE,
    DRAWCIRCLE,
    SELECT_E
} Status;

class openglwidget : public QGLWidget
{
public:
    openglwidget(QWidget *parent = NULL);
    ~openglwidget();
    void initializeGL();
    void paintGL();
    void setAction(Status s);

private:
    Data *data;
    Status status;
    MetaElement metaElement;
    Line *verticalDottedLine;
    Line *horizonalDottedLine;

protected:
    void resizeGL(int w, int h);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool isHorizontal(float y1, float y2);
    bool isVertical(float x1, float x2);
    bool isParallelToAnotherLine(float x11, float y11, float *x21, float *y21);
    bool isDiagonal(float *x1, float *y1, float x2, float y2);
    bool pointIsCloseToAnother(float *x, float *y);
    void paintDottedLines();
};

#endif // OPENGLWIDGET_H

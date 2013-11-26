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

// Necesarry declarations for linker
class Data;

// Drawer state enumerator
enum Status
{
    DRAWLINE,
    DRAWCIRCLE
};

class openglwidget : public QGLWidget
{
public:
    openglwidget(QWidget *parent = NULL);
    ~openglwidget();
    void initializeGL();
    void paintGL();

private:
    Data *data;
    Status status;
    MetaElement metaElement;

protected:
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // OPENGLWIDGET_H

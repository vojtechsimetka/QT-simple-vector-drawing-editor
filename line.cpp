/**
 * PGR 2013 project
 * Line representation in editor
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    line.cpp
 */
#include "line.h"
#include <QDebug>

/**
 * @brief Line constructor
 * @param x1 First point's coordinate x
 * @param y1 First point's coordinate y
 * @param x2 Second point's coordinate x
 * @param y2 Second point's coordinate y
 */
Line::Line(float x1, float y1, float x2, float y2)
    : p1(x1,y1),
      p2(x2,y2)
{
}

/**
 * @brief Paints line to OpenGL context
 */
void Line::PaintMe() const
{
    // Sets color
    glColor3f(0.0, 0.0, 0.0);

    // Draws line
    glBegin(GL_LINES);
    glVertex2f(this->p1.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p2.getY());
    glEnd();
}

void Line::HighlightMe()
{
    ;
}

void Line::resize(float x1, float y1, float x2, float y2)
{
    this->p1.setX(x1);
    this->p1.setY(y1);
    this->p2.setX(x2);
    this->p2.setY(y2);
}

Point Line::getP1() const
{
    return p1;
}

void Line::setP1(const Point &value)
{
    p1 = value;
}
Point Line::getP2() const
{
    return p2;
}

void Line::setP2(const Point &value)
{
    p2 = value;
}



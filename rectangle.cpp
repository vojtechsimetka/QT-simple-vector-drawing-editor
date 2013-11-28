/**
 * PGR 2013 project
 * Rectangle representation in editor
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    rectangle.cpp
 */
#include "rectangle.h"
#include <QDebug>

/**
 * @brief Rectangle constructor
 * @param x Upper left corner's coordinate x
 * @param y Upper left corner's coordinate y
 * @param w Rectangle's width
 * @param h Rectangle's height
 */
Rectangle::Rectangle(float x1, float y1, float x2, float y2)
    :p1(x1,y1),
     p2(x2,y2)
{
}

/**
 * @brief Paints rectangle to OpenGL context
 */
void Rectangle::PaintMe() const
{
    // Sets color
    glColor3f(0.0, 0.0, 0.0);

    // Draws rectangle
    glBegin(GL_LINES);
    // Top
    glVertex2f(this->p1.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p1.getY());
    // Right
    glVertex2f(this->p2.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p2.getY());
    // Bottom
    glVertex2f(this->p2.getX(), this->p2.getY());
    glVertex2f(this->p1.getX(), this->p2.getY());
    // Left
    glVertex2f(this->p1.getX(), this->p2.getY());
    glVertex2f(this->p1.getX(), this->p1.getY());
    glEnd();
}

void Rectangle::PaintPoints() const
{
    Point::paintPoint(this->p1);
    Point::paintPoint(this->p2);
    Point::paintPoint(this->p1.getX(), this->p2.getY());
    Point::paintPoint(this->p2.getX(), this->p1.getY());
}

/**
 * @brief Resize rectangle
 * @param x1 First point's coordinate x
 * @param y1 First point's coordinate y
 * @param x2 Second point's coordinate x
 * @param y2 Second point's coordinate y
 */
void Rectangle::resize(float x1, float y1, float x2, float y2)
{
    this->p1.setX(x1);
    this->p1.setY(y1);
    this->p2.setX(x2);
    this->p2.setY(y2);

}
Point Rectangle::getP1() const
{
    return p1;
}

void Rectangle::setP1(const Point &value)
{
    p1 = value;
}
Point Rectangle::getP2() const
{
    return p2;
}

void Rectangle::setP2(const Point &value)
{
    p2 = value;
}







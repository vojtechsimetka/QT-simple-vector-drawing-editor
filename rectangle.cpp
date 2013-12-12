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
#include <QRect>

/**
 * @brief Rectangle constructor
 * @param x Upper left corner's coordinate x
 * @param y Upper left corner's coordinate y
 * @param w Rectangle's width
 * @param h Rectangle's height
 */
Rectangle::Rectangle(float x1, float y1, float x2, float y2)
    : p1(x1,y1)
    , p2(x2,y1)
    , p3(x2,y2)
    , p4(x1,y2)
{
}

Rectangle::~Rectangle()
{

}

/**
 * @brief Paints rectangle to OpenGL context
 */
void Rectangle::paintMe() const
{
    // Sets color
    glLineWidth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    glColor4f(0.5, 0.75, 0.95, 0.7);

    // Draws rectangle
    glBegin(GL_LINES);
    // Top
    glVertex2f(this->p1.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p2.getY());
    // Right
    glVertex2f(this->p2.getX(), this->p2.getY());
    glVertex2f(this->p3.getX(), this->p3.getY());
    // Bottom
    glVertex2f(this->p3.getX(), this->p3.getY());
    glVertex2f(this->p4.getX(), this->p4.getY());
    // Left
    glVertex2f(this->p4.getX(), this->p4.getY());
    glVertex2f(this->p1.getX(), this->p1.getY());
    glEnd();

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

/**
 * @brief Paints points of the rectangle
 */
void Rectangle::paintPoints(float x, float y) const
{
    Point::paintPoint(this->p1, x,y);
    Point::paintPoint(this->p2, x,y);
    Point::paintPoint(this->p3, x,y);
    Point::paintPoint(this->p4, x,y);
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
    this->p2.setY(y1);
    this->p3.setX(x2);
    this->p3.setY(y2);
    this->p4.setX(x1);
    this->p4.setY(y2);

}

/**
 * @brief Gets first point of the rectangle
 * @return First rectangle point
 */
Point Rectangle::getP1() const
{
    return p1;
}

/**
 * @brief Sets rectangle's first point to new value
 * @param value New first point's coordinates
 */
void Rectangle::setP1(const Point &value)
{
    p1 = value;
}

/**
 * @brief Gets second point of the rectangle
 * @return Second rectangle point
 */
Point Rectangle::getP2() const
{
    return p2;
}

/**
 * @brief Sets rectangle's second point to new value
 * @param value New second point's coordinates
 */
void Rectangle::setP2(const Point &value)
{
    p2 = value;
}


bool Rectangle::getCounterPoint(float x, float y, float *ox, float *oy, Qt::Corner *orientation) const
{
    if (Point::isNearby(x, y, this->p1.getX(), this->p1.getY()))
    {
        *ox = this->p3.getX();
        *oy = this->p3.getY();
        *orientation = Qt::TopLeftCorner;
        return true;
    }

    else if (Point::isNearby(x, y, this->p2.getX(), this->p2.getY()))
    {
        *ox = this->p4.getX();
        *oy = this->p4.getY();
        *orientation = Qt::TopRightCorner;
        return true;
    }

    else if (Point::isNearby(x, y, this->p3.getX(), this->p3.getY()))
    {
        *ox = this->p1.getX();
        *oy = this->p1.getY();
        *orientation = Qt::BottomRightCorner;
        return true;
    }

    else if (Point::isNearby(x, y, this->p4.getX(), this->p4.getY()))
    {
        *ox = this->p2.getX();
        *oy = this->p2.getY();
        *orientation = Qt::BottomLeftCorner;
        return true;
    }

    return false;
}

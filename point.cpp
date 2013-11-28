/**
 * PGR 2013 project
 * Implements 2D point in float
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    point.cpp
 */
#include "point.h"
#include "openglwidget.h"

/**
 * @brief Point constructor
 * @param x Coordinate x
 * @param y Coordinate y
 */
Point::Point(float x, float y)
{
    this->x = x;
    this->y = y;
}

/**
 * @brief Returns coordinate x
 * @return Coordinate x
 */
float Point::getX() const
{
    return x;
}

/**
 * @brief Sets coordinate x to new value
 * @param x New x coordinate
 */
void Point::setX(float x)
{
    this->x = x;
}

/**
 * @brief Returns coodinate y
 * @return Coordinate y
 */
float Point::getY() const
{
    return y;
}

/**
 * @brief Sets coodinate y to new value
 * @param y New y coordinate
 */
void Point::setY(float y)
{
    this->y = y;
}

/**
 * @brief Sets point's new location
 * @param x New x coordinate
 * @param y New y coordinate
 */
void Point::setLocation(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Point::paintPoint(Point p)
{
    Point::paintPoint(p.getX(), p.getY());
}

void Point::paintPoint(float x, float y)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(5);
    glColor4f(0.5, 0.75, 0.95, 0.5);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();

    glDisable(GL_BLEND);
}






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






/**
 * PGR 2013 project
 * Implements manipulation with lines that are used for assisting with painting
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    guidingline.cpp
 */
#include "guideline.h"

/**
 * @brief GuidingLine constructor
 * @param red   Red color intensity
 * @param green Green color intensity
 * @param blue  Blue color intensity
 */
GuideLine::GuideLine(float red, float green, float blue)
    : Line(0,0,0,0)
{
    this->valid = false;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

/**
 * @brief Sets line's points
 * @param x1 Coordinate x of point 1
 * @param y1 Coordinate y of point 1
 * @param x2 Coordinate x of point 2
 * @param y2 Coordinate y of point 2
 */
void GuideLine::set(float x1, float y1, float x2, float y2)
{
    this->setP1(x1,y1);
    this->setP2(x2,y2);
    this->valid = true;
}

void GuideLine::resizeTo(float x, float y)
{
    this->setP1(x,y);
    this->valid = true;
}

/**
 * @brief Paints guiding line
 */
void GuideLine::paintMe() const
{
    // Line is not valid, do not draw
    if (!this->valid)
        return;

    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);

    // Sets color
    glColor3f(this->red, this->green, this->blue);

    // Draw primitive
    glBegin(GL_LINES);
    glVertex2f(this->getP1().getX(), this->getP1().getY());
    glVertex2f(this->getP2().getX(), this->getP2().getY());
    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

/**
 * @brief Invalidates guiding line
 */
void GuideLine::invalidate()
{
    this->valid = false;
}
bool GuideLine::isValid() const
{
    return valid;
}

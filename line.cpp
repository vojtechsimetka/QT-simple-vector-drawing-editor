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
void Line::paintMe() const
{
    // Line is selected, paint blue line around
    if (this->selected)
    {
        glLineWidth(6.0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);

        glColor4f(0.5, 0.75, 0.95, 0.7);

        // Draws line
        glBegin(GL_LINES);
        glVertex2f(this->p1.getX(), this->p1.getY());
        glVertex2f(this->p2.getX(), this->p2.getY());
        glEnd();

        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }

    // Sets color
    if (this->highlighted)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 0.0, 0.0);

    glLineWidth(2.0);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    // Draws line
    glBegin(GL_LINES);
    glVertex2f(this->p1.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p2.getY());

    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);

    // If selected, paint points
    if (this->selected)
        this->paintPoints();
}

/**
 * @brief Paints line's points
 */
void Line::paintPoints() const
{
    Point::paintPoint(this->p1);
    Point::paintPoint(this->p2);
}

/**
 * @brief Resizes line
 * @param x1 First point's coordinate x
 * @param y1 First point's coordinate y
 * @param x2 Second point's coordinate x
 * @param y2 Second point's coordinate y
 */
void Line::resize(float x1, float y1, float x2, float y2)
{
    this->p1.setX(x1);
    this->p1.setY(y1);
    this->p2.setX(x2);
    this->p2.setY(y2);
}

/**
 * @brief Gets first line point
 * @return First line point
 */
Point Line::getP1() const
{
    return p1;
}

/**
 * @brief Sets first line point's new location
 * @param x New coordinate x
 * @param y New coordinate y
 */
void Line::setP1(float x, float y)
{
    this->p1.setLocation(x, y);
}

/**
 * @brief Gets second line point
 * @return Second line point
 */
Point Line::getP2() const
{
    return p2;
}

/**
 * @brief Sets second line point's new location
 * @param x New coordinate x
 * @param y New coordinate y
 */
void Line::setP2(float x, float y)
{
    this->p2.setLocation(x, y);
}

/**
 * @brief Tests if line and selection rectangle intersects
 * @param rect Selection rectangle reference
 * @param offset Scene offset
 * @return True if the line and a selection rectangle intersects
 */
bool Line::intersects(SelectionRectangle rect, Point offset)
{
    // TODO: REMAKE TO INTERSECTION WITH LINE AND RECTANGLE
    return rect.intersects(this->getP1(), offset) ||
           rect.intersects(this->getP2(), offset);
}


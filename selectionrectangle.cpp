/**
 * PGR 2013 project
 * Selection rectangle implementation
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/29
 * @version 1
 * @file    selectionrectangle.cpp
 */
#include "selectionrectangle.h"

/**
 * @brief SelectionRectangle constructor
 */
SelectionRectangle::SelectionRectangle()
    : min_x(0)
    , min_y(0)
    , max_x(0)
    , max_y(0)
    , active (false)
{
}

/**
 * @brief Paints rectangle
 */
void SelectionRectangle::paintMe()
{
    // Selection is not active
    if (!this->active)
        return;

    // Enables blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setst color
    glColor4f(0.5, 0.75, 0.95, 0.7);

    // Fill's rectangle
    glBegin(GL_QUADS);
    glVertex2f(this->min_x, this->min_y);
    glVertex2f(this->max_x, this->min_y);
    glVertex2f(this->max_x, this->max_y);
    glVertex2f(this->min_x, this->max_y);
    glEnd();

    // Disables blending
    glDisable(GL_BLEND);

    // Draws rectangle's borders
    glBegin(GL_LINES);
    // Top
    glVertex2f(this->min_x, this->min_y);
    glVertex2f(this->max_x, this->min_y);
    // Right
    glVertex2f(this->max_x, this->min_y);
    glVertex2f(this->max_x, this->max_y);
    // Bottom
    glVertex2f(this->max_x, this->max_y);
    glVertex2f(this->min_x, this->max_y);
    // Left
    glVertex2f(this->min_x, this->max_y);
    glVertex2f(this->min_x, this->min_y);
    glEnd();
}

/**
 * @brief Resizes and activates selection rectangle from diagonal
 * @param x1 Coordinate x of first point
 * @param y1 Coordinate y of first point
 * @param x2 Coordinate x of second point
 * @param y2 Coordinate y of second point
 */
void SelectionRectangle::resize(float x1, float y1, float x2, float y2, const Point offset, float scale)
{
    this->width = fabs(x1 - x2);
    this->height = fabs(y1 - y2);

    this->min_x = x1 < x2 ? x1 : x2;
    this->min_y = y1 < y2 ? y1 : y2;
    this->max_x = min_x + this->width;
    this->max_y = min_y + this->height;

    this->active = true;

    this->transform(offset, scale);
}

/**
 * @brief Deactivates selection rectangle
 */
void SelectionRectangle::deactivate()
{
    this->active = false;
}

/**
 * @brief Tests if rectangle intersects with point
 * @param point Point to intersect with
 * @return True if point and rectangle intersects
 */
bool SelectionRectangle::intersects(Point point)
{
    return this->intersects(point.getX(), point.getY());
}

/**
 * @brief Tests if rectangle intersects with point
 * @param x Point's coordinate x
 * @param y Point's coordinate y
 * @return True if point and rectangle intersects
 */
bool SelectionRectangle::intersects(float x, float y)
{
    return this->translated_min_x < x &&
           this->translated_max_x > x &&
           this->translated_min_y < y &&
           this->translated_max_y > y;
}

/**
 * @brief Transforms rectangle to match model's offset
 * @param offset Offset of the scene
 * @param scale Scale of the scene
 */
void SelectionRectangle::transform(Point offset, float scale)
{
    this->translated_min_x = (this->min_x / scale) - offset.getX();
    this->translated_max_x = (this->max_x / scale) - offset.getX();
    this->translated_min_y = (this->min_y / scale) - offset.getY();
    this->translated_max_y = (this->max_y / scale) - offset.getY();
}

bool SelectionRectangle::intersects(const Element *e)
{
    return e->intersects(this->translated_min_x,
                         this->translated_min_y,
                         this->translated_max_x,
                         this->translated_max_y);
}

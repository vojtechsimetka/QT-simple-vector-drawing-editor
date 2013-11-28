/**
 * PGR 2013 project
 * Used for drawing and resizing elements in scene
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    metaelement.cpp
 */
#include "metaelement.h"
#include <QDebug>

/**
 * @brief MetaElement constructor
 */
MetaElement::MetaElement()
    :origin(0,0)
{
    this->clear();
}

/**
 * @brief MetaElement destructor
 */
MetaElement::~MetaElement()
{
}

/**
 * @brief Tests if there any element is being drawn or resized
 * @return True if an element is being drawn or resized
 */
bool MetaElement::isEmpty()
{
    return this->element == NULL;
}

/**
 * @brief Initializes metaelement for drawing/resizing
 * @param element Element to be modified
 * @param x Origin coordinate x
 * @param y Origin coordinate y
 */
void MetaElement::init(Element *element, float x, float y)
{
    this->element = element;
    this->origin.setX(x);
    this->origin.setY(y);
}

/**
 * @brief Paints the element that is being modified
 */
void MetaElement::paintMe()
{
    if (this->element != NULL)
        this->element->PaintMe();
}

/**
 * @brief Resizes element from origin to point with coordinates x and y
 * @param x Coordinate x for resizing
 * @param y Coordinate y for resizing
 */
void MetaElement::resizeTo(float x, float y)
{
    this->element->resize(x,y,origin.getX(),origin.getY());
}

/**
 * @brief Clears the metaelement
 */
void MetaElement::clear()
{
    this->element = NULL;
}

/**
 * @brief Gets element that is being modified
 * @return Element being modified
 */
Element *MetaElement::getElement()
{
    return this->element;
}
Point MetaElement::getOrigin() const
{
    return origin;
}


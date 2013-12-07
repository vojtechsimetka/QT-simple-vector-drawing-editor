/**
 * PGR 2013 project
 * Abstract class used for all objects that can be drawn
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    element.cpp
 */
#include "element.h"

/**
 * @brief Element constructor
 */
Element::Element(ElementType::Type type)
    : highlighted(false)
    , selected(false)
    , type(type)
    , scale_x(1)
    , scale_y(1)
    , offset_x(0)
    , offset_y(0)
{

}

Element::~Element()
{

}

/**
 * @brief Highlights element
 */
void Element::highlightMe()
{
    highlighted = true;
}

/**
 * @brief Dehighlights element
 */
void Element::deHighlightMe()
{
    highlighted = false;
}

/**
 * @brief Marks element as selected
 */
void Element::selectMe()
{
    selected = true;
}

/**
 * @brief Deselects element
 */
void Element::deSelectMe()
{
    selected = false;
}

ElementType::Type Element::getType() const
{
    return this->type;
}

bool Element::isSelected() const
{
    return this->selected;
}

void Element::setScaleAndTranslation(float scale_x, float scale_y, float offset_x, float offset_y)
{
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->offset_x = offset_x;
    this->offset_y = offset_y;
}

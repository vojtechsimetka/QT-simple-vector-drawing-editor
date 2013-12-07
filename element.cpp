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

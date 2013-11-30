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
Element::Element()
    : highlighted(false)
    , selected(false)
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

/**
 * PGR 2013 project
 * Data class for editor, stores all elements and logs changes
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    data.cpp
 */
#include "data.h"
#include <QDebug>

/**
 * @brief Data constructor
 */
Data::Data()
{
}

/**
 * @brief Create and adds line to data structure
 * @param x1 Coordinate x of point 1
 * @param y1 Coordinate y of point 1
 * @param x2 Coordinate x of point 2
 * @param y2 Coordinate y of point 2
 */
void Data::createLine(float x1, float y1, float x2, float y2)
{
    // Creates and stores line
    Line *l = new Line(x1, y1, x2, y2);
    this->elements.push_back(l);
}

/**
 * @brief Paints all elements from data structure
 */
void Data::paintAll()
{
    foreach (Element* e, this->elements)
        e->paintMe();
}

/**
 * @brief Adds element to data structure and logs its addition
 * @param e Element to be added
 */
void Data::add(Element * e)
{
    // Stores element
    this->elements.push_back(e);

}

/**
 * @brief Remove specified element
 * @param e Element to remove
 */
void Data::remove(Element *e)
{
    // Erases element
    std::vector<Element *>::iterator it = std::find(this->elements.begin(), this->elements.end(), e);
    if(it != this->elements.end())
        this->elements.erase(it);  
}

/**
 * @brief Dehighlight all elemets
 */
void Data::deHighlightAll()
{
    foreach (Element *e, this->elements) {
        e->deHighlightMe();
    }
}

/**
 * @brief Gets all elements stored in data structure
 * @return Elements vector
 */
std::vector<Element *> Data::getElements()
{
    return elements;
}


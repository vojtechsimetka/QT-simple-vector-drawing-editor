/**
 * PGR 2013 project
 * Data class for editor, stores all elements and logs changes
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    data.h
 */
#ifndef DATA_H
#define DATA_H

#include <QLinkedList>
#include "line.h"
#include "rectangle.h"

class Data
{
public:
    Data();
    void createLine(float, float, float, float);
    void createRectangle(float, float, float, float);
    void paintAll();
    void add(Element *e);
    void remove(Element *e);
    std::vector<Element *> getElements();
    void deHighlightAll();

private:
    std::vector<Element*> elements;
};

#endif // DATA_H

#ifndef DATA_H
#define DATA_H

#include <QLinkedList>
#include "line.h"

class Data
{
public:
    Data();
    void createLine(float, float, float, float);
    void paintAll();
    void add(Element *e);

private:
    QLinkedList<Element*> elements;
};

#endif // DATA_H

/**
 * PGR 2013 project
 * Implements manipulation with lines that are used for assisting with painting
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/25
 * @version 1
 * @file    guidingline.h
 */
#ifndef GUIDINGLINES_H
#define GUIDINGLINES_H

#include "line.h"

//class Line;

class GuideLine: public Line
{
public:
    GuideLine(float red, float green, float blue);
//    bool isValid();
    void set(float, float, float, float);
    void paintMe() const;
    void invalidate();

private:
    bool valid;
};

#endif // GUIDINGLINES_H

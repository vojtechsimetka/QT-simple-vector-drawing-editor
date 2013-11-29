#ifndef GUIDINGLINES_H
#define GUIDINGLINES_H

#include "line.h"

//class Line;

class GuidingLine: public Line
{
public:
    GuidingLine(float red, float green, float blue);
    bool isValid();
    void set(float, float, float, float);
    void PaintMe() const;
    void invalidate();

private:
    bool valid;
};

#endif // GUIDINGLINES_H

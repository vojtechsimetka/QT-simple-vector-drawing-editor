#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
public:
    virtual void PaintMe() const = 0;
    virtual void HighlightMe() = 0;
    virtual void resize(float x1, float y1, float x2, float y2) = 0;
};

#endif // ELEMENT_H

#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
public:
    inline Element() { this->highlighted = false; }
    virtual void PaintMe() const = 0;
    virtual void PaintPoints() const = 0;
    inline void highlightMe() { highlighted = true; }
    inline void deHighlightMe() { highlighted = false; }
    virtual void resize(float x1, float y1, float x2, float y2) = 0;

protected:
    bool highlighted;
};

#endif // ELEMENT_H

#include "guidingline.h"

GuidingLine::GuidingLine(float red, float green, float blue)
    : Line(0,0,0,0)
{
    this->valid = false;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

bool GuidingLine::isValid()
{
    return this->valid;
}

void GuidingLine::set(float x1, float y1, float x2, float y2)
{
    this->setP1(x1,y1);
    this->setP2(x2,y2);
    this->valid = true;
}

void GuidingLine::PaintMe() const
{
    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);

    glColor3f(this->red, this->green, this->blue);
    glBegin(GL_LINES);
    glVertex2f(this->getP1().getX(), this->getP1().getY());
    glVertex2f(this->getP2().getX(), this->getP2().getY());
    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

void GuidingLine::invalidate()
{
    this->valid = false;
}

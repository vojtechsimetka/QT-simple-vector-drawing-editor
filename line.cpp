/**
 * PGR 2013 project
 * Line representation in editor
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    line.cpp
 */
#include "line.h"
#include <QDebug>
#include "openglwidget.h"

/**
 * @brief Line constructor
 * @param x1 First point's coordinate x
 * @param y1 First point's coordinate y
 * @param x2 Second point's coordinate x
 * @param y2 Second point's coordinate y
 */
Line::Line(float x1, float y1, float x2, float y2)
    : Element(ElementType::LINE)
    , p1(x1,y1)
    , p2(x2,y2)
    , o_p1(x1,y1)
    , o_p2(x2,y2)
{
}

Line::~Line()
{

}

/**
 * @brief Paints line to OpenGL context
 */
void Line::paintMe() const
{
    // Line is selected, paint blue line around
    if (this->selected)
    {
        glLineWidth(6.0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);

        glColor4f(0.5, 0.75, 0.95, 0.7);

        // Draws line
        glBegin(GL_LINES);
        glVertex2f(this->p1.getX(), this->p1.getY());
        glVertex2f(this->p2.getX(), this->p2.getY());
        glEnd();

        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }

    // Sets color
    if (this->highlighted)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 0.0, 0.0);

    glLineWidth(2.0);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    // Draws line
    glBegin(GL_LINES);
    glVertex2f(this->p1.getX(), this->p1.getY());
    glVertex2f(this->p2.getX(), this->p2.getY());

    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

/**
 * @brief Paints line's points
 */
void Line::paintPoints() const
{
    Point::paintPoint(this->p1);
    Point::paintPoint(this->p2);
}

/**
 * @brief Resizes line
 * @param x1 First point's coordinate x
 * @param y1 First point's coordinate y
 * @param x2 Second point's coordinate x
 * @param y2 Second point's coordinate y
 */
void Line::resize(float x1, float y1, float x2, float y2)
{
    this->p1.setX(x1);
    this->p1.setY(y1);
    this->p2.setX(x2);
    this->p2.setY(y2);

    this->o_p1.setX(x1);
    this->o_p1.setY(y1);
    this->o_p2.setX(x2);
    this->o_p2.setY(y2);
}

/**
 * @brief Gets first line point
 * @return First line point
 */
Point Line::getP1() const
{
    return p1;
}

/**
 * @brief Sets first line point's new location
 * @param x New coordinate x
 * @param y New coordinate y
 */
void Line::setP1(float x, float y)
{
    this->p1.setLocation(x, y);
    this->o_p1.setLocation(x, y);
}

/**
 * @brief Gets second line point
 * @return Second line point
 */
Point Line::getP2() const
{
    return p2;
}

/**
 * @brief Sets second line point's new location
 * @param x New coordinate x
 * @param y New coordinate y
 */
void Line::setP2(float x, float y)
{
    this->p2.setLocation(x, y);
    this->o_p2.setLocation(x, y);
}

/**
 * @brief Tests if line intersects with rectangle alligned with axis x an y
 * @param min_x Left border of rectangle
 * @param min_y Bottom borner of rectangle
 * @param max_x Right corner of rectangle
 * @param max_y Top corner of rectangle
 * @return True if line and rectangle intersects
 */
bool Line::intersects(float min_x, float min_y, float max_x, float max_y) const
{
    double t0 = 0.0;
    double t1 = 1.0;
    double d_x = this->getP2().getX()-this->getP1().getX();
    double d_y = this->getP2().getY()-this->getP1().getY();
    double p, q, r;

    // Goes through alel four edges of rectangle
    for(int edge = 0; edge < 4; edge++)
    {
        if (edge == 0)
        {
            p = -d_x;
            q = -(min_x - this->getP1().getX());
        }
        if (edge == 1)
        {
            p = d_x;
            q = (max_x - this->getP1().getX());
        }
        if (edge == 2)
        {
            p = -d_y;
            q = -(min_y - this->getP1().getY());
        }
        if (edge == 3)
        {
            p = d_y;
            q = (max_y - this->getP1().getY());
        }
        r = q / p;

        // Line is parallel with some border and lies outside
        if(p == 0 && q < 0)
            return false;

        if(p < 0)
        {
            if(r > t1)
                return false;

            // Line and rectangle possibly intersects
            else if(r > t0)
                t0 = r;
        }
        else if(p > 0)
        {
            if(r < t0)
                return false;

            // Line and rectangle possibly intersects
            else if(r < t1)
                t1 = r;
        }
    }

    // Line intersects with rectangle
    if (t0 < t1)
        return true;


    return false;
}

float Line::length() const
{
    return pow(p2.getX() - p1.getX(),2.) + pow(p2.getY() - p1.getY(),2.);
}

float Line::distanceFromPoint(float x, float y) const
{
    float len = this->length();

    // Distance to point
    if (len == 0) return sqrt(pow(x - p1.getX(),2.) + pow(y - p1.getY(),2.));

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = ((x - this->p1.getX()) * (this->p2.getX() - this->p1.getX())
               + (y - this->p1.getY()) * (this->p2.getY() - this->p1.getY())) / len;

    if (t < 0) return sqrt(pow(x - this->p1.getX(),2.) + pow(y - this->p1.getY(),2.));
    if (t > 1) return sqrt(pow(x - this->p2.getX(),2.) + pow(y - this->p2.getY(),2.));

    float pomx = this->p1.getX() + t * (this->p2.getX() - this->p1.getX());
    float pomy = this->p1.getY() + t * (this->p2.getY() - this->p1.getY());

    return sqrt(pow(x - pomx,2.) + pow(y - pomy,2.));
}

/**
 * @brief Tests mutual orientation of three point
 * @param p1 First point
 * @param p2 Second point
 * @param p3 Third point
 * @return
 */
int Line::orientation(Point p1, Point p2, Point p3) const
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    int val = (p2.getY() - p1.getY()) * (p3.getX() - p2.getX()) -
              (p2.getX() - p1.getX()) * (p3.getY() - p2.getY());

    // Points are colinear
    if (val == 0)
        return 0;

    // Clock-wise or counterclock-wise
    return (val > 0)? 1: 2;
}

/**
 * @brief Tests if point q lies on a segment p1p2
 * @param p1 First point of the segment
 * @param q Point to be tested
 * @param p2 Second point of the segment
 * @return True if point 2 lies on a segment p1p2
 */
bool Line::onSegment(Point p1, Point q, Point p2) const
{
    // Point q is on the segment p1p2
    if (q.getX() <= fmax(p1.getX(), p2.getX()) &&
        q.getX() >= fmin(p1.getX(), p2.getX()) &&
        q.getY() <= fmax(p1.getY(), p2.getY()) &&
        q.getY() >= fmin(p1.getY(), p2.getY()))
       return true;

    return false;
}

/**
 * @brief Tests if this line intersects if another
 * @param line Line to be tested for intersection with this line
 * @return True if lines intersects
 */
bool Line::intersects(Line *line) const
{
    // Find the four orientations needed for general and special cases
    int o1 = orientation(this->p1, line->getP1(), this->p2);
    int o2 = orientation(this->p1, line->getP1(), line->getP2());
    int o3 = orientation(this->p2, line->getP2(), this->p1);
    int o4 = orientation(this->p2, line->getP2(), line->getP1());

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, line->getP1()))
        return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, line->getP2(), line->getP1()))
        return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, line->getP2()))
        return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, line->getP1(), line->getP2()))
        return true;

    // Doesn't fall in any of the above cases
    return false;
}

/**
  * @brief Tests if line and point intersects
  * @param p Point to be tested if intersects with line
  * @return True if line and point intersects
  */
 bool Line::intersects(Point p) const
 {
     // If distance from line and point is lower then treshold value return true
     if (this->distanceFromPoint(p.getX(), p.getY()) < OpenGLWidget::treshold_value)
         return true;

     return false;
 }

 bool Line::intersects(float x, float y) const
 {
     // If distance from line and point is lower then treshold value return true
     if (this->distanceFromPoint(x, y) < OpenGLWidget::treshold_value)
         return true;

     return false;
 }

 /**
 * @brief If point with coordinates x and y is nearby a control point, return the other one
 * @param x Coordinate x of point where user clicked
 * @param y Coordinate y of point where user clicked
 * @param ox Output coordinate x of opposite control point - if found
 * @param oy Output coordinate y of opposite control point - if found
 * @return True if opposite control point was found
 */
bool Line::getCounterPoint(float x, float y, float * ox, float * oy) const
 {
    // User clicked nearby first control point, return second control point
    if (Point::isNearby(x, y, this->p1.getX(), this->p1.getY()))
    {
        *ox = p2.getX();
        *oy = p2.getY();
        return true;
    }

    // User clicked nearby second control point, return first control point
    if (Point::isNearby(x, y, this->p2.getX(), this->p2.getY()))
    {
        *ox = p1.getX();
        *oy = p1.getY();
        return true;
    }

    // User clicked somewhere else, return false
    return false;
 }

float Line::getMinX() const
{
    return this->p1.getX() < this->p2.getX() ? this->getP1().getX() : this->p2.getX();
}

float Line::getMinY() const
{
    return this->p1.getY() < this->p2.getY() ? this->getP1().getY() : this->p2.getY();
}

float Line::getMaxX() const
{
    return this->p1.getX() > this->p2.getX() ? this->getP1().getX() : this->p2.getX();
}

float Line::getMaxY() const
{
    return this->p1.getY() > this->p2.getY() ? this->getP1().getY() : this->p2.getY();
}

void  Line::resizeToBoundingRectangle(float min_x, float min_y, float max_x, float max_y)
{
    if (this->o_p1.getX() < this->o_p2.getX())
    {
        this->p1.setX(min_x);
        this->p2.setX(max_x);
    }

    else
    {
        this->p1.setX(max_x);
        this->p2.setX(min_x);
    }

    if (this->o_p1.getY() < this->o_p2.getY())
    {
        this->p1.setY(min_y);
        this->p2.setY(max_y);
    }

    else
    {
        this->p1.setY(max_y);
        this->p2.setY(min_y);
    }
}

void Line::finalizeResize()
{
    this->o_p1.setLocation(this->p1.getX(), this->p1.getY());
    this->o_p2.setLocation(this->p2.getX(), this->p2.getY());
}

void Line::translatef(float x, float y)
{
    this->p1.setLocation(this->p1.getX() + x, this->p1.getY() + y);
    this->p2.setLocation(this->p2.getX() + x, this->p2.getY() + y);
}

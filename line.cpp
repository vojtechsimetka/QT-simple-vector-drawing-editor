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

    // If selected, paint points
    if (this->selected)
        this->paintPoints();
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

bool Line::lineIntersection(float, float, float, float, float, float)
{
    return false;
}

 bool Line::intersects(Point p) const
 {
     if (this->distanceFromPoint(p.getX(), p.getY()) < OpenGLWidget::treshold_value)
         return true;
     return false;
 }

bool Line::getCounterPoint(float x, float y, float * ox, float * oy) const
 {
     if (this->p1.distance(x,y) < 3*OpenGLWidget::treshold_value)
     {
         *ox = p2.getX();
         *oy = p2.getY();
         return true;
     }

     else if (this->p2.distance(x,y) < 3*OpenGLWidget::treshold_value)
     {
         *ox = p1.getX();
         *oy = p1.getY();
         return true;
     }

     return false;
 }


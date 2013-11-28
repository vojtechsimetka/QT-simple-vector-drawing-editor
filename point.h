/**
 * PGR 2013 project
 * Implements 2D point in float
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    point.h
 */
#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point(float x, float y);
    float getX() const;
    void setX(float x);
    float getY() const;
    void setY(float y);
    void setLocation(float x, float y);
    static void paintPoint(Point p);
    static void paintPoint(float x, float y);

private:
    float x;
    float y;

};

#endif // POINT_H

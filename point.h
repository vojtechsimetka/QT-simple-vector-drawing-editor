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
    float distance(float x, float y) const;
    static float distance(float x1, float y1, float x2, float y2);
    static bool isNearby(float x1, float y1, float x2, float y2);

private:
    float x;
    float y;

};

#endif // POINT_H

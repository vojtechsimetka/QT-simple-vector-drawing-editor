/**
 * PGR 2013 project
 * Texture-mapped text.
 * Inspired by http://10098.arnet.am/?p=118
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 *          Grigory
 *
 * @date    2011/25/4
 * @version 1
 * @file    font.h
 */

#ifndef FONT_H
#define FONT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <QGLWidget>

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif


class Font
{
public:
    Font();
    void render(const char* c, float x, float y);
private:
    GLuint fontTexture;
    GLuint dl;
    void loadTexture();
    QImage glTextureImg;

};

#endif // FONT_H


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


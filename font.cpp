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
 * @file    font.cpp
 */

#include "font.h"
#include <QtDebug>

class QImage;

Font::Font()
{
    this->loadTexture();
}

/*
 * Builds the font from a bitmap file.
 * The entire file is read as a texture and then we create display lists
 * for each printable character. Those lists just create a small quad, applying
 * the relevant piece of the texture to it.
 */
void Font::loadTexture()
{
    // Load image with texture
    QImage textImg;
    if (!textImg.load(":/font/font/font.bmp"))
    {
        return;
    }

    // Convert image to GL Format
    glTextureImg = QGLWidget::convertToGLFormat(textImg);

    // Generate GL Texture
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, glTextureImg.width(), glTextureImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glTextureImg.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // Now compile the glyphs into display lists
    dl = glGenLists(96);
    for(char c = ' '; c <= '~'; ++c){
      int idx = c - ' ';
      glNewList(dl+idx, GL_COMPILE);
         glBegin(GL_QUADS);
         glTexCoord2i(8*idx, 0);
         glVertex2i(0, 0);

         glTexCoord2i(8*idx + 8, 0);
         glVertex2i(8, 0);

         glTexCoord2i(8*idx + 8, 8);
         glVertex2i(8, 8);

         glTexCoord2i(8*idx, 8);
         glVertex2i(0, 8);
         glEnd();
      glEndList();
    }
}


/*
 * Renders a string on the screen
 */
void Font::render(const char* c, float x, float y)
{
    // Prepare OpenGL
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    // Modify texture matrix
    glScalef(1.0/glTextureImg.width(), -1.0/glTextureImg.height(), 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Draw all characters
    for(int i = 0; i < strlen(c); ++i)
    {
      int idx = c[i] - ' ';
      if(idx < 0 || idx >= 96) throw std::runtime_error("Non-printable character encountered");
      glPushMatrix();
      glTranslatef(x + 8*i, y, 0);
      glCallList(dl + idx);
      glPopMatrix();
    }

   glMatrixMode(GL_TEXTURE);
   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glMatrixMode(GL_PROJECTION);
}


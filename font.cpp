#include "font.h"
#include <QtDebug>

class QImage;

Font::Font()
{
    this->loadTexture();
}

void Font::loadTexture()
{
    QImage textImg;
    if (!textImg.load(":/font/font/font.bmp"))
    {
        return;
    }

    glTextureImg = QGLWidget::convertToGLFormat(textImg);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, glTextureImg.width(), glTextureImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glTextureImg.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

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


void Font::render(const char* c, float x, float y)
{
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glScalef(1.0/glTextureImg.width(), -1.0/glTextureImg.height(), 1.0);
    glMatrixMode(GL_MODELVIEW);

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


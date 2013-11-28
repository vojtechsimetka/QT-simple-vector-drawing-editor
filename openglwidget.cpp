/**
 * PGR 2013 project
 * OpenGL widget class responsible for displaying drawing
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    openglwidget.cpp
 */
#include "openglwidget.h"
#include <QtDebug>

/**
 * @brief openglwidget::openglwidget
 * @param parent Reference to parent component
 */
openglwidget::openglwidget(QWidget *parent):
    QGLWidget(parent)
{
    // Initializes status
    this->status = SELECT_E;

    // Creates data set
    this->data = new Data();

    // Sets mouse to be tracked even without any mouse button pressed
    this->setMouseTracking(true);
}

/**
 * @brief openglwidget::~openglwidget
 */
openglwidget::~openglwidget()
{
    // Unallocate data
    delete this->data;
}

/**
 * @brief Initializes OpenGL
 */
void openglwidget::initializeGL()
{
    // Sets backround color
    qglClearColor(QColor::fromRgb(255, 255, 255));

    // Sets which matrix stack is used
    glMatrixMode(GL_PROJECTION);

    // Resizes OpenGL window
    this->resizeGL(500,500);

    // Disables Z-buffer
    glDisable(GL_DEPTH_TEST);
}

/**
 * @brief Repaints scene
 */
void openglwidget::paintGL()
{
    // Clean the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Paints all elements on screen
    data->paintAll();

    // Paints element that is being drawn or resized
    this->metaElement.paintMe();
}

/**
 * @brief Resizes OpenGL window
 * @param w New width
 * @param h New height
 */
void openglwidget::resizeGL(int w, int h)
{
    glLoadIdentity();
    glOrtho (0, w, h, 0, 0, 1);
}

/**
 * @brief Mouse pressed event handler
 * @param event Reference to event descriptor
 */
void openglwidget::mouseReleaseEvent(QMouseEvent *event)
{
    // There isn't any element that is being modified
    if (this->metaElement.isEmpty())
    {
        Element *e = NULL;

        // Determine what to do
        switch (this->status)
        {
        case DRAWLINE:
            // Create new line
            e = new Line(event->x(),event->y(),event->x(),event->y());
            break;
        case DRAWRECTANGLE:
            // Create new rectangle
            e = new Rectangle(event->x(),event->y(),event->x(),event->y());
            break;

        default:
            break;
        }

        // Some element was created, set it for modification to metaelement
        if (e != NULL)
            this->metaElement.init(e,event->x(),event->y());
    }

    else
    {
        switch (this->status)
        {
        case SELECT_E:
            break;
        case DRAWRECTANGLE:
        case DRAWCIRCLE:
        case DRAWLINE:
            // Add element to data structure
            this->data->add(metaElement.getElement());

            // Clear information in metaelement
            this->metaElement.clear();
            break;
        default:
            break;

        }

    }

    // Repaint scene
    this->repaint();
}

/**
 * @brief Mouse moved event handler
 * @param event Reference to event descriptor
 */
void openglwidget::mouseMoveEvent(QMouseEvent *event)
{
    // Save mouse coordinates
    float x = event->x();
    float y = event->y();

    // Some element is being modified, change its size
    if (!this->metaElement.isEmpty())
    {
        switch (this->status)
        {
        case SELECT_E:
            break;
        case DRAWCIRCLE:
        case DRAWRECTANGLE:
        case DRAWLINE:
            // Save lines origin point
            int x1 = this->metaElement.getOrigin().getX();
            int y1 = this->metaElement.getOrigin().getY();

            // Check if painted line is almost horizontal
            if (isHorizontal(y1,y))
                y = y1;
            // Check if painted line is almost vertical
            if (isVertical(x1,x))
                x = x1;

            break;
        }


        this->metaElement.resizeTo(x,y);
    }

    // Repaint scene
    this->repaint();
}

/**
 * @brief Find out if line specified with two coordinates is almost horizontal
 * @param y1 First point's coordinate y
 * @param y2 Second point's coordinate y
 */
bool openglwidget::isHorizontal(float y1, float y2)
{
    if (fabs(y2 - y1) < MINDISTANCE)
        return true;
    return false;
}

/**
 * @brief Find out if line specified with two coordinates is almost vertical
 * @param x1 First point's coordinate x
 * @param x2 Second point's coordinate x
 */
bool openglwidget::isVertical(float x1, float x2)
{
    if (fabs(x2 - x1) < MINDISTANCE)
        return true;
    return false;
}

/**
 * @brief openglwidget::setAction
 * @param s
 */
bool openglwidget::isParallelToAnotherLine(float x1, float x2, float y1, float y2)
{

}

void openglwidget::setAction(Status s)
{
    this->status = s;
}

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
    // TODO: Change to select when ready
    this->status = DRAWRECTANGLE;

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
void openglwidget::mousePressEvent(QMouseEvent *event)
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
        // Resize element that is being modified
        this->metaElement.resizeTo(event->x(),event->y());

        // Add element to data structure
        this->data->add(metaElement.getElement());

        // Clear information in metaelement
        this->metaElement.clear();
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
    // Some element is being modified, change its size
    if (!this->metaElement.isEmpty())
        this->metaElement.resizeTo(event->x(),event->y());

    // Repaint scene
    this->repaint();
}

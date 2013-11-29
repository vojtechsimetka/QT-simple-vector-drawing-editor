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
 * @brief OpenGL Widget constructor
 * @param parent Reference to parent component
 */
openglwidget::openglwidget(QWidget *parent):
    QGLWidget(parent)
{
    // Initializes status
    this->status = SELECT_E;

    // Creates data set
    this->data = new Data();

    // Init dotted line
    this->verticalDottedLine = new GuidingLine(1,0,0);
    this->horizonalDottedLine = new GuidingLine(0,1,0);

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
    glClearColor(1,1,1,1);

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

    // Paints dotted line - ukazuje k cemu se to chyta..
    this->paintDottedLines();
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
    // Save mouse point
    float x = event->x();
    float y = event->y();

    // There isn't any element that is being modified
    if (this->metaElement.isEmpty())
    {
        Element *e = NULL;

        // Determine what to do
        switch (this->status)
        {
        case DRAWLINE:
            // Catch to close point
            pointIsCloseToAnother(&x,&y);
            // Create new line
            e = new Line(x,y,x,y);
            break;
        case DRAWRECTANGLE:
            // Create new rectangle
            e = new Rectangle(x,y,x,y);
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

            // Dehighlight all elements
            this->data->deHighlightAll();

            // Clear dotted lines
            this->verticalDottedLine->invalidate();
            this->horizonalDottedLine->invalidate();

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
            float x1 = this->metaElement.getOrigin().getX();
            float y1 = this->metaElement.getOrigin().getY();

            // Check if painted line is almost horizontal
            if (isParallelToAnotherLine(x1,y1,&x,&y))
                ;
            else if (isHorizontal(y1,y))
                y = y1;
            // Check if painted line is almost vertical
            else if (isVertical(x1,x))
                x = x1;
            else if (this->isDiagonal(&x, &y, x1, y1))
                ;
            pointIsCloseToAnother(&x,&y);

            break;
        }


        this->metaElement.resizeTo(x,y);
    }
    else
    {
        if (this->status == DRAWLINE)
            pointIsCloseToAnother(&x,&y);
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

bool openglwidget::isDiagonal(float *x1, float *y1, float x2, float y2)
{
    // Axis of 2nd and 4th quadrant
    if (fabs((*x1 - x2) - (*y1 - y2)) < MINDISTANCE)
    {
        int d = *x1 - x2;

        *x1 = x2 + d;
        *y1 = y2 + d;
        return true;
    }

    // Axis of 1st and 3th quadrant
    else if (fabs((x2 - *x1) - (*y1 - y2)) < MINDISTANCE)
    {
        int d = x2 - *x1;

        *x1 = x2 - d;
        *y1 = y2 + d;
        return true;
    }
    return false;
}


bool openglwidget::isParallelToAnotherLine(float x11, float y11, float *x21, float *y21)
{
    // Get all elements in window
    std::vector<Element *> allElements = this->data->getElements();
    std::vector<Element *>::iterator it = allElements.begin();

    bool parallelFound = false;

    // Find if some line is parallel to current painted line
    for (; it != allElements.end(); ++it) {
        // First ratio
        float firstRatio = (*y21 - y11) / (*x21 - x11);
        // Get line
        Line *e = (Line *)(*it);
        // Get lines coordinates
        float x12 = e->getP1().getX();
        float y12 = e->getP1().getY();
        float x22 = e->getP2().getX();
        float y22 = e->getP2().getY();
        // Second ratio
        float secondRatio = (y22 - y12) / (x22 - x12);

        // first ratio is almost equal to second ratio
        if (fabs(secondRatio - firstRatio) < 0.1) {
            // Highlight lines
            e->highlightMe();

            if (!parallelFound)
            {
                // Change y
                if (firstRatio < 1) {
                    // firstRatio must be equal to secondRatio
                    *y21 = (secondRatio * (*x21 - x11)) + y11;
                }
                // Change x
                else {
                    // firstRatio must be equal to secondRatio
                    *x21 = ((*y21 - y11) / secondRatio) + x11;
                }
                parallelFound = true;
                this->metaElement.highlightMe();
            }
        } else {
            // Dehighlight line
            e->deHighlightMe();
        }
    }

    if (!parallelFound) this->metaElement.deHighlightMe();
    return parallelFound;
}

/**
 * @brief Find out if poit is close to another
 * @param x Point's X coordinate
 * @param y Point's Y coordinate
 * @return true or false
 */
bool openglwidget::pointIsCloseToAnother(float *x, float *y)
{
    // Get all elements in window
    std::vector<Element *> allElements = this->data->getElements();
    std::vector<Element *>::iterator it = allElements.begin();

    bool closeXPointFound = false;
    bool closeYPointFound = false;

    // Find if some line is parallel to current painted line
    for (; it != allElements.end(); ++it) {
        // Get line
        Line *e = (Line *)(*it);

        // Get lines coordinates
        float x1 = e->getP1().getX();
        float y1 = e->getP1().getY();
        float x2 = e->getP2().getX();
        float y2 = e->getP2().getY();

        if (fabs(x1 - *x) < MINDISTANCE)
        {
            *x = x1;
            this->verticalDottedLine->set(*x, *y, x1, y1);
            closeXPointFound = true;
        }
        else if (fabs(x2 - *x) < MINDISTANCE)
        {
            *x = x2;
            this->verticalDottedLine->set(*x, *y, x2, y2);
            closeXPointFound = true;
        }

        if (fabs(y1 - *y) < MINDISTANCE)
        {
            *y = y1;
            this->horizonalDottedLine->set(*x, *y, x1, y1);
            this->verticalDottedLine->set(*x,*y, this->verticalDottedLine->getP2().getX(), this->verticalDottedLine->getP2().getY()); // VYZKOUSET!!!

            closeYPointFound = true;
        }
        else if (fabs(y2 - *y) < MINDISTANCE)
        {
            *y = y2;
            this->horizonalDottedLine->set(*x,*y, x2,y2);
            this->verticalDottedLine->set(*x,*y, this->verticalDottedLine->getP2().getX(), this->verticalDottedLine->getP2().getY()); // VYZKOUSET!!!
            closeYPointFound = true;
        }
    }

    if (!closeXPointFound)
        this->verticalDottedLine->invalidate();

    if (!closeYPointFound)
        this->horizonalDottedLine->invalidate();

    return (closeXPointFound || closeYPointFound);
}

/**
 * @brief Paint dotted line
 */
void openglwidget::paintDottedLines()
{
    if (this->verticalDottedLine->isValid())
        this->verticalDottedLine->PaintMe();

    if (this->horizonalDottedLine->isValid())
        this->horizonalDottedLine->PaintMe();

//    this->verticalDottedLine->PaintMe();
//    this->horizonalDottedLine->PaintMe();


//    // get first line's coordinates
//    float x1 = this->verticalDottedLine->getP1().getX();
//    float y1 = this->verticalDottedLine->getP1().getY();
//    float x2 = this->verticalDottedLine->getP2().getX();
//    float y2 = this->verticalDottedLine->getP2().getY();

//    // get second line's coordinates
//    float x12 = this->horizonalDottedLine->getP1().getX();
//    float y12 = this->horizonalDottedLine->getP1().getY();
//    float x22 = this->horizonalDottedLine->getP2().getX();
//    float y22 = this->horizonalDottedLine->getP2().getY();

//    glColor3f(0.0, 1.0, 0.0);
//    glLineStipple(1, 0xAAAA);
//    glEnable(GL_LINE_STIPPLE);

//    glBegin(GL_LINES);
//    glVertex2f(x1,y1);
//    glVertex2f(x2,y2);
//    glEnd();

//    glColor3f(1.0, 0.0, 0.0);
//    glBegin(GL_LINES);
//    glVertex2f(x12,y12);
//    glVertex2f(x22,y22);
//    glEnd();

//    glDisable(GL_LINE_STIPPLE);
}


/**
 * @brief openglwidget::setAction
 * @param s
 */
void openglwidget::setAction(Status s)
{
    this->status = s;
}

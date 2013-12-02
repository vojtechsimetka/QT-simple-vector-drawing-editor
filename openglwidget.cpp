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
#include "math.h"
#include "changeslog.h"
#include <QtDebug>

/**
 * @brief OpenGL Widget constructor
 * @param parent Reference to parent component
 */
OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , offset(0, 0)
    , aux_offset(0, 0)
    , mouse_start_position(0, 0)
    , mouse_end_position(0, 0)
    , scale(1)
    , treshold_value(MINDISTANCE)
{
    // Initializes status
    this->status = SELECT_E;

    // Creates data set
    this->data = new Data();

    // Init Changes log
    ChangesLog::sharedInstance()->init(this->data);

    // Init dotted line
    this->vertical_guideline = new GuideLine(1,0,0);
    this->horizontal_guideline = new GuideLine(0,1,0);

    // Sets mouse to be tracked even without any mouse button pressed
    this->setMouseTracking(true);
}

/**
 * @brief OpenGL Widget destructor
 */
OpenGLWidget::~OpenGLWidget()
{
    // Unallocate data
    delete this->data;
}

/**
 * @brief Initializes OpenGL
 */
void OpenGLWidget::initializeGL()
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
void OpenGLWidget::paintGL()
{
    // Saves matrix
    glPushMatrix();

    glScalef(this->scale, this->scale, this->scale);

    // Translates scene by offset
    glTranslatef(this->offset.getX(),
                 this->offset.getY(),
                 0);

    // Clean the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Paints all elements on screen
    data->paintAll();

    // Paints element that is being drawn or resized
    this->metaElement.paintMe();

    // Paints guiding lines
    this->vertical_guideline->paintMe();
    this->horizontal_guideline->paintMe();

    // Restores matrix context
    glPopMatrix();

    // Draws selection rectangle
    this->selection_rectangle.paintMe();
}

/**
 * @brief Resizes OpenGL window
 * @param w New width
 * @param h New height
 */
void OpenGLWidget::resizeGL(int w, int h)
{
    glLoadIdentity();
    glOrtho (0, w, h, 0, 0, 1);
}

/**
 * @brief Mouse pressed event handler
 * @param event Reference to event descriptor
 */
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Save mouse point
    float x = (event->x() / this->scale) - this->offset.getX();
    float y = (event->y() / this->scale) - this->offset.getY();

    // Left mouse button release
    if (event->button() == Qt::LeftButton)
    {
        switch(this->status)
        {
        case SELECT_E:
            // Deactivates selection rectangle
            this->selection_rectangle.deactivate();

            // Selects all elements within selection rectangle
            foreach (Element *e, this->data->getElements())
            {
                if (this->selection_rectangle.intersects(e))
                {
                    this->selected_items.push_back(e);
                    e->selectMe();
                }
                else
                    e->deSelectMe();
            }

            break;
        case DRAWLINE:
        case DRAWRECTANGLE:
            this->mouseReleaseDraw(x, y);
            break;
        default:
            break;
        }
    }

    // Right mouse button release
    else if (event->button() == Qt::RightButton)
        this->aux_offset.setLocation(0, 0);

    // Repaint scene
    this->repaint();
}

/**
 * @brief Mouse pressed event handler
 * @param event Reference to event descriptor
 */
void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    // Stores mouse position for drawing selection rectangle
    this->mouse_start_position.setLocation(event->x(), event->y());
    this->mouse_end_position.setLocation(event->x(), event->y());

    // Left mouse button pressed
    if (event->buttons() & Qt::LeftButton)
    {
        switch(this->status)
        {
        case SELECT_E:
            //TODO: test jestli jsem naohoud neklikl na nejaky objekt -> posun objektu

            // Deselects all element
            foreach (Element * e, this->selected_items)
                e->deSelectMe();

            // Clears selected items
            this->selected_items.clear();

            // Starts displaying selection rectangle
            this->selection_rectangle.resize(this->mouse_start_position.getX(),
                                             this->mouse_start_position.getY(),
                                             this->mouse_end_position.getX(),
                                             this->mouse_end_position.getY(),
                                             this->offset,
                                             this->scale);
        default:
            break;
        }
    }
}

/**
 * @brief Implements drawing in mouse release
 * @param x Coordinate x in model's coordinates system
 * @param y Coordinate y in model's coordinates system
 */
void OpenGLWidget::mouseReleaseDraw(float x, float y)
{
    // There isn't any element that is being drawn
    if (this->metaElement.isEmpty())
        this->createNewElement(x, y);

    // There is element being drawn, finish it
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
            // Logs addition
            ChangesLog::sharedInstance()->doStep(ADD,0,0,metaElement.getElement());

            // Dehighlight all elements
            this->data->deHighlightAll();

            // Clear dotted lines
            this->vertical_guideline->invalidate();
            this->horizontal_guideline->invalidate();

            // Clear information in metaelement
            this->metaElement.clear();

            // Starts new element from same coordinates where we've finnished
            this->catchToClosePoint(&x,&y);
            this->createNewElement(x, y);
            break;

        default:
            break;
        }

    }
}

/**
 * @brief Creates new element that is being drawn
 * @param x Coordinate x of the newly created element
 * @param y Coordinate y of the newly created element
 */
void OpenGLWidget::createNewElement(float x, float y)
{
    Element *e = NULL;

    // Determine what to do
    switch (this->status)
    {
    case DRAWLINE:
        // Catch to close point
        catchToClosePoint(&x,&y);
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
        this->metaElement.init(e,x,y);
}

/**
 * @brief Keyboard pressed event handler
 * @param keyEvent Reference to event descriptor
 */
void OpenGLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    // Key was pressed
    if (keyEvent->key())
    {
        // Space was pressed
        if (Qt::Key_Space)
        {
            // Stop with drawing lines
            if (this->status == DRAWLINE)
            {
                this->metaElement.clear();
                this->repaint();
            }
        }
    }
}

/**
 * @brief Mouse wheel event handler
 * @param event Reference to event descriptor
 */
void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    // Sets scale to new
    float delta_scale = (float)(event->delta())/1000;
    this->scale += delta_scale;

    // Limit minimal maxification
    if (this->scale < 0.1)
    {
        this->scale = 0.1;
        this->treshold_value = MINDISTANCE / this->scale;
        return;
    }

    // Limit maximal magnification
    else if (this->scale > 50)
    {
        this->scale = 50;
        this->treshold_value = MINDISTANCE / this->scale;
        return;
    }

    // Reevaluates treshold value used for guiding functions
    this->treshold_value = MINDISTANCE / this->scale;

    // Alternates offset so that zooming mouse cursor stays on same place
    this->offset.setLocation(this->offset.getX() - (delta_scale * event->x())/this->scale,
                             this->offset.getY() - (delta_scale * event->y())/this->scale);
    // Repaints scene
    this->repaint();
}

/**
 * @brief Mouse moved event handler
 * @param event Reference to event descriptor
 */
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Save mouse coordinates
    float x = (event->x() / this->scale) - this->offset.getX();
    float y = (event->y() / this->scale) - this->offset.getY();
    this->mouse_end_position.setLocation(event->x(), event->y());

    // Left button
    if (event->buttons() & Qt::RightButton)
    {
        // Determines how far the mouse was dragged
        float x1 = (event->x() - this->mouse_start_position.getX()) / this->scale;
        float y1 = (event->y() - this->mouse_start_position.getY()) / this->scale;

        // Determines change in x and y according to previous drag
        float dx = (this->aux_offset.getX() - x1);
        float dy = (this->aux_offset.getY() - y1);

        // Sets previous drag
        this->aux_offset.setLocation(this->aux_offset.getX() - dx,
                                     this->aux_offset.getY() - dy);

        // Sets new offset
        this->offset.setLocation(this->offset.getX() - dx,
                                 this->offset.getY() - dy);
    }

    // Right button
    else if (event->button() & Qt::LeftButton ||
             event->button() == 0)
    {
        // Some element is being modified, change its size
        if (!this->metaElement.isEmpty())
        {
            switch (this->status)
            {
            case SELECT_E:
                break;
            case DLT:
                break;
            case PAN:
                break;
            case ROTATE:
                break;
            case DRAWCIRCLE:
            case DRAWRECTANGLE:
            case DRAWLINE:
                // Save lines origin point
                float x1 = this->metaElement.getOrigin().getX();
                float y1 = this->metaElement.getOrigin().getY();

                catchLenght(x1,y1,&x,&y);

                // Check if painted line is almost parallel to another line
                if (catchToParallelLine(x1,y1,&x,&y))
                    ;

                if (catchToPerpendicular(x1,y1,&x,&y))
                    ;
                // Check if painted line is almost horizontal
                else if (isHorizontal(y1,y))
                    y = y1;
                // Check if painted line is almost vertical
                else if (isVertical(x1,x))
                    x = x1;
                // Check if painted line is almost diagonal
                else if (this->catchToDiagonal(&x, &y, x1, y1))
                    ;
                // Try to catch point to another very close point
                catchToClosePoint(&x,&y);

                break;
            }

            // Resizes element
            this->metaElement.resizeTo(x,y);
        }

        // There is not any element being modified or drawn
        else
        {
            // Create new line
            if (this->status == DRAWLINE)
                catchToClosePoint(&x,&y);

            // Draw selection rectangle
            else if (this->status == SELECT_E &&
                     event->buttons() & Qt::LeftButton)
            {
                this->selection_rectangle.resize(this->mouse_start_position.getX(),
                                                 this->mouse_start_position.getY(),
                                                 this->mouse_end_position.getX(),
                                                 this->mouse_end_position.getY(),
                                                 this->offset,
                                                 this->scale);

                // Select or deselect element
                foreach (Element *e, this->data->getElements())
                {
                    if (this->selection_rectangle.intersects(e))
                        e->selectMe();

                    else
                        e->deSelectMe();
                }
            }
        }
    }

    // Repaint scene
    this->repaint();
}

/**
 * @brief Find out if line specified with two coordinates is almost horizontal
 * @param y1 First point's coordinate y
 * @param y2 Second point's coordinate y
 */
bool OpenGLWidget::isHorizontal(float y1, float y2)
{
    if (fabs(y2 - y1) < this->treshold_value)
        return true;
    return false;
}

/**
 * @brief Find out if line specified with two coordinates is almost vertical
 * @param x1 First point's coordinate x
 * @param x2 Second point's coordinate x
 */
bool OpenGLWidget::isVertical(float x1, float x2)
{
    if (fabs(x2 - x1) < this->treshold_value)
        return true;
    return false;
}

/**
 * @brief Catches line to diagonal
 * @param x1 Coordinate x of point of origin
 * @param y1 Coordinate y of point of origin
 * @param x2 Coordinate x of second point
 * @param y2 Coordinate y of second point
 * @return True if element catched to a diagonal
 */
bool OpenGLWidget::catchToDiagonal(float *x1, float *y1, float x2, float y2)
{
    // Axis of 2nd and 4th quadrant
    if (fabs((*x1 - x2) - (*y1 - y2)) < this->treshold_value)
    {
        int d = *x1 - x2;

        *x1 = x2 + d;
        *y1 = y2 + d;
        return true;
    }

    // Axis of 1st and 3th quadrant
    else if (fabs((x2 - *x1) - (*y1 - y2)) < this->treshold_value)
    {
        int d = x2 - *x1;

        *x1 = x2 - d;
        *y1 = y2 + d;
        return true;
    }
    return false;
}

/**
 * @brief Tests if there is a nearly perpendicular line to the line being drawn, makes drawn line perpendicular
 * @param x11 Coordinate x of point of origin for line being drawn
 * @param y11 Coordinate y of point of origin for line being drawn
 * @param x21 Coordinate x of second point for line being drawn
 * @param y21 Coordinate y of second point for line being drawn
 * @return True if there is nearly perpendicular line to the one being drawn
 */
bool OpenGLWidget::catchToPerpendicular(float x11, float y11, float *x21, float *y21)
{
    // Direction of first line
        float k1 = (*y21 - y11) / (*x21 - x11);

        // Find if some line is perpendicular to current painted line
        foreach (Element *element, this->data->getElements())
        {
            Line *e = (Line *)element;

            // Get lines coordinates
            float x12 = e->getP1().getX();
            float y12 = e->getP1().getY();
            float x22 = e->getP2().getX();
            float y22 = e->getP2().getY();

            if (!(((x12 == x11) && (y12 == y11))
                    || ((x22 == x11) && (y22 == y11))))
                continue;

            // Direction of second line
            float k2 = (y22 - y12) / (x22 - x12);

            if (fabs(k1 + (1/k2)) < 0.5)
            {
                if ((x12 == x11) && (y12 == y11))
                {
                    // move x
                    if (k1 > 1)
                    {
                        *x21 = (-k2 * (*y21 - y11)) + x11;
                    }
                    else
                    {
                        *y21 = ((*x21 - x11) / -k2) + y11;
                    }
                }
            }
    }
}

void OpenGLWidget::catchLenght(float x11, float y11, float *x21, float *y21)
{
    float lenght1 = sqrt(pow(*x21 - x11,2.) + pow(*y21 - y11,2.));

    float k = fabs((*y21 - y11) / (*x21 - x11));

    // Find if some line is parallel to current painted line
    foreach (Element *element, this->data->getElements())
    {
        Line *e = (Line *) element;

        // Get lines coordinates
        float x12 = e->getP1().getX();
        float y12 = e->getP1().getY();
        float x22 = e->getP2().getX();
        float y22 = e->getP2().getY();

        float lenght2 = sqrt(pow(x22 - x12,2.) + pow(y22 - y12,2.));

        if (fabs(lenght2 - lenght1) < this->treshold_value)
        {
            float dx = sqrt(pow(lenght2-lenght1,2.) / (1 + pow(k,2.)));
            float dy = k * dx;

            // 1st quadrant
            if ((*x21 > x11) && (*y21 >= y11))
            {
                if (lenght2 > lenght1)
                {
                    *x21 += dx;
                    *y21 += dy;
                } else
                {
                    *x21 -= dx;
                    *y21 -= dy;
                }

            }
            // 3rd quadrant
            else if ((*x21 < x11) && (*y21 <= y11))
            {
                if (lenght2 > lenght1)
                {
                    *x21 -= dx;
                    *y21 -= dy;
                } else
                {
                    *x21 += dx;
                    *y21 += dy;
                }
            }

            // 2nd quadrant
            else if ((*x21 <= x11) && (*y21 > y11))
            {
                if (lenght2 > lenght1)
                {
                    *x21 -= dx;
                    *y21 += dy;
                } else
                {
                    *x21 += dx;
                    *y21 -= dy;
                }
            }

            // 4th quadrant
            else if ((*x21 >= x11) && (*y21 < y11))
            {
                if (lenght2 > lenght1)
                {
                    *x21 += dx;
                    *y21 -= dy;
                } else
                {
                    *x21 -= dx;
                    *y21 += dy;
                }
            }

        }
    }
}


/**
 * @brief Tests if there is a nearly parallel line to the line being drawn, makes drawn line parallel
 * @param x11 Coordinate x of point of origin for line being drawn
 * @param y11 Coordinate y of point of origin for line being drawn
 * @param x21 Coordinate x of second point for line being drawn
 * @param y21 Coordinate y of second point for line being drawn
 * @return True if there is nearly parallel line to the one being drawn
 */
bool OpenGLWidget::catchToParallelLine(float x11, float y11, float *x21, float *y21)
{
    bool parallelFound = false;

    // Find if some line is parallel to current painted line
    foreach (Element *element, this->data->getElements())
    {
        // First ratio
        float firstRatio = (*y21 - y11) / (*x21 - x11);
        // Get line
        Line *e = (Line *) element;

        // Get lines coordinates
        float x12 = e->getP1().getX();
        float y12 = e->getP1().getY();
        float x22 = e->getP2().getX();
        float y22 = e->getP2().getY();
        // Second ratio
        float secondRatio = (y22 - y12) / (x22 - x12);

        // First ratio is almost equal to second ratio
        if (fabs(secondRatio - firstRatio) < 0.1)
        {
            // Highlight lines
            e->highlightMe();

            // This is first parallel line found, change coordinates of the line being drawn
            if (!parallelFound)
            {
                // Change y
                if (firstRatio < 1)
                    // firstRatio must be equal to secondRatio
                    *y21 = (secondRatio * (*x21 - x11)) + y11;

                // Change x
                else
                    // firstRatio must be equal to secondRatio
                    *x21 = ((*y21 - y11) / secondRatio) + x11;

                parallelFound = true;
                this->metaElement.highlightMe();
            }
        }
        // Lines are not parallel, gehighlight
        else
            e->deHighlightMe();
    }

    // There isn't any parallel line to the one being drawn, dehighlight it
    if (!parallelFound)
        this->metaElement.deHighlightMe();

    return parallelFound;
}

/**
 * @brief Find out if poit is close to another
 * @param x Point's X coordinate
 * @param y Point's Y coordinate
 */
void OpenGLWidget::catchToClosePoint(float *x, float *y)
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

        if (fabs(x1 - *x) < this->treshold_value)
        {
            *x = x1;
            this->vertical_guideline->set(*x, *y, x1, y1);
            closeXPointFound = true;
        }
        else if (fabs(x2 - *x) < this->treshold_value)
        {
            *x = x2;
            this->vertical_guideline->set(*x, *y, x2, y2);
            closeXPointFound = true;
        }

        if (fabs(y1 - *y) < this->treshold_value)
        {
            *y = y1;
            this->horizontal_guideline->set(*x, *y, x1, y1);
            this->vertical_guideline->set(*x,*y, this->vertical_guideline->getP2().getX(), this->vertical_guideline->getP2().getY()); // VYZKOUSET!!!

            closeYPointFound = true;
        }
        else if (fabs(y2 - *y) < this->treshold_value)
        {
            *y = y2;
            this->horizontal_guideline->set(*x,*y, x2,y2);
            this->vertical_guideline->set(*x,*y, this->vertical_guideline->getP2().getX(), this->vertical_guideline->getP2().getY()); // VYZKOUSET!!!
            closeYPointFound = true;
        }
    }

    if (!closeXPointFound)
        this->vertical_guideline->invalidate();

    if (!closeYPointFound)
        this->horizontal_guideline->invalidate();
}


/**
 * @brief Set tool to selected one
 * @param tool Selected tool enumerator
 */
void OpenGLWidget::setAction(Status tool)
{
    this->status = tool;
    this->horizontal_guideline->invalidate();
    this->vertical_guideline->invalidate();
    this->selection_rectangle.deactivate();
    this->metaElement.clear();
}

/**
 * @brief Deletes selected items from the data structure
 */
void OpenGLWidget::deleteSelection()
{
    // Delete selected items from data
    foreach (Element *e, this->selected_items)
        data->remove(e);

    // Logs deletion
    ChangesLog::sharedInstance()->doStep(DELETE,0,0,new std::list<Element *>(this->selected_items));

    // Clear selection
    this->selected_items.clear();

    // Repaint scene
    this->repaint();
}

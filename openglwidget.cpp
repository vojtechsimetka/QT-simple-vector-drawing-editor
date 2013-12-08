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
#include "mainwindow.h"
#include <QtDebug>

float OpenGLWidget::treshold_value = MINDISTANCE;

/**
 * @brief OpenGL Widget constructor
 * @param parent Reference to parent component
 */
OpenGLWidget::OpenGLWidget(Gui *gui, QWidget *parent)
    : QGLWidget(parent)
    , offset(0, 0)
    , aux_offset(0, 0)
    , mouse_start_position(0, 0)
    , mouse_end_position(0, 0)
    , scale(1)
    , selection(new SelectionRectangle(gui))
    , gui(gui)
{
    // Initializes status
    this->status = SELECT_E;
    this->catchStatus = CLASSIC;

    // Creates data set
    this->data = new Data();

    // Init Changes log
    ChangesLog::sharedInstance()->init(this->data, this->selection);

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

    // Deletes selection
    delete this->selection;
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

    // Load font
    this->font = new Font();
}

/**
 * @brief Repaints scene
 */
void OpenGLWidget::paintGL()
{
    // Saves matrix
    glPushMatrix();

    glScalef(this->scale, this->scale, 1);

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

    this->selection->paintBoundingRectangle(this->translateX(this->mouse_end_position.getX()),
                                            this->translateY(this->mouse_end_position.getY()));

    // Restores matrix context
    glPopMatrix();

    // Draws selection rectangle
    this->selection->paintMe();

    this->paintRuler();
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
 * @brief Mouse pressed  handler
 * @param event Reference to event descriptor
 */
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Save mouse point
    float x = this->translateX(event->x());
    float y = this->translateY(event->y());

    // Right mouse button release or pan tool and left mouse button release
    if (event->button() == Qt::RightButton ||
        (event->button() == Qt::LeftButton &&
        this->status == PAN))
        this->aux_offset.setLocation(0, 0);

    // Left mouse button release
    else if (event->button() == Qt::LeftButton)
    {
        switch(this->status)
        {
        case SELECT_E:
            this->mouseReleaseSelect();
            break;

        case DRAW:
            this->mouseReleaseDraw(x, y);
            break;

        default:
            break;
        }
    }

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
            try
            {
            this->mousePressSelect();
            }
            catch (QString* str)
            {
                qDebug()<<*str;
            }

            break;

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
    {
        this->createNewElement(x, y);
        this->gui->setUndo(false);
        this->gui->setRedo(false);
    }

    // There is element being drawn, finish it
    else
    {
        // Save coordinates
        Line *l = (Line *)this->metaElement.getElement();
        x = l->getP2().getX();
        y = l->getP2().getY();

        switch (this->type)
        {
        case ElementType::CIRCLE:
        case ElementType::POLYGON:
        case ElementType::LINE:
            // Add element to data structure
            this->data->add(metaElement.getElement());
            // Logs addition
            ChangesLog::sharedInstance()->doStep(ADD,0,0,metaElement.getElement());

            // Disables redo and enables undo
            this->gui->setRedo(false);

            // Dehighlight all elements
            this->data->deHighlightAll();

            // Clear dotted lines
            this->vertical_guideline->invalidate();
            this->horizontal_guideline->invalidate();

            // Clear information in metaelement
            this->metaElement.clear();

            if (this->catchStatus != CLASSIC)
                this->catchStatus = CLASSIC;

            // Starts new element from same coordinates where we've finnished
            this->createNewElement(x, y);
            break;

        case ElementType::SELECTION_RECTANGLE:
            throw QString("There shall never be instance of selection rectangle as a drawable object");
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
    switch (this->type)
    {
    case ElementType::LINE:
        // Catch to close point
        catchToClosePoint(&x,&y);
        // Create new line
        e = new Line(x,y,x,y);
        break;

    case ElementType::CIRCLE:
        break;

    case ElementType::POLYGON:
        break;
    case ElementType::SELECTION_RECTANGLE:
        throw QString("There shall never be instance of selection rectangle as a drawable object");
        break;
    }

    // Some element was created, set it for modification to metaelement
    if (e != NULL)
        this->metaElement.init(e,x,y);
}

QString OpenGLWidget::qKeyEventToQString(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_0:
    case Qt::Key_Launch0:
        return "0";
        break;

    case Qt::Key_1:
    case Qt::Key_Launch1:
        return "1";
        break;

    case Qt::Key_2:
    case Qt::Key_Launch2:
        return "2";
        break;

    case Qt::Key_3:
    case Qt::Key_Launch3:
        return "3";
        break;

    case Qt::Key_4:
    case Qt::Key_Launch4:
        return "4";
        break;

    case Qt::Key_5:
    case Qt::Key_Launch5:
        return "5";
        break;

    case Qt::Key_6:
    case Qt::Key_Launch6:
        return "6";
        break;

    case Qt::Key_7:
    case Qt::Key_Launch7:
        return "7";
        break;

    case Qt::Key_8:
    case Qt::Key_Launch8:
        return "8";
        break;

    case Qt::Key_9:
    case Qt::Key_Launch9:
        return "9";
        break;
    }

    return "";
}

/**
 * @brief Keyboard pressed event handler
 * @param keyEvent Reference to event descriptor
 */
void OpenGLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    static float len = 0;
    // Key was pressed
    switch(keyEvent->key())
    {
    case Qt::Key_Space:
        // Stop with drawing lines
        this->catchStatus = CLASSIC;
        if (this->status == DRAW && this->type == ElementType::LINE)
        {
            if (ChangesLog::sharedInstance()->canUndo())
                this->gui->setUndo(true);
            this->metaElement.clear();
            this->data->deHighlightAll();
            this->vertical_guideline->invalidate();
            this->horizontal_guideline->invalidate();
            this->gui->setLineEditText("");
            this->repaint();
        }
        break;

    case Qt::Key_P:
        this->data->deHighlightAll();
        this->vertical_guideline->invalidate();
        this->horizontal_guideline->invalidate();
        if (this->catchStatus != PARALLEL)
            this->catchStatus = PARALLEL;
        else this->catchStatus = CLASSIC;
        break;
    case Qt::Key_R:
        this->data->deHighlightAll();
        this->vertical_guideline->invalidate();
        this->horizontal_guideline->invalidate();
        if (this->catchStatus != PERPENDICULAR)
            this->catchStatus = PERPENDICULAR;
        else this->catchStatus = CLASSIC;
        break;
    case Qt::Key_M:
        this->data->deHighlightAll();
        this->vertical_guideline->invalidate();
        this->horizontal_guideline->invalidate();
        if (this->catchStatus != MIDDLE)
            this->catchStatus = MIDDLE;
        else this->catchStatus = CLASSIC;
        break;

    case Qt::Key_0:
    case Qt::Key_Launch0:

    case Qt::Key_1:
    case Qt::Key_Launch1:

    case Qt::Key_2:
    case Qt::Key_Launch2:

    case Qt::Key_3:
    case Qt::Key_Launch3:

    case Qt::Key_4:
    case Qt::Key_Launch4:

    case Qt::Key_5:
    case Qt::Key_Launch5:

    case Qt::Key_6:
    case Qt::Key_Launch6:

    case Qt::Key_7:
    case Qt::Key_Launch7:

    case Qt::Key_8:
    case Qt::Key_Launch8:

    case Qt::Key_9:
    case Qt::Key_Launch9:

        // Switch state to changeSize - stop drawing
        if (this->status == DRAW && this->type == ElementType::LINE)
        {
            this->status = CHANGESIZE;
            len = this->gui->getLineEditText().toFloat();
            this->gui->setLineEditText("");
        }
        // Set new text
        if (this->status == CHANGESIZE)
            this->gui->appendTextToLineEdit(this->qKeyEventToQString(keyEvent));
        break;

    case Qt::Key_Enter:
    case Qt::Key_Q:
        if (this->status == CHANGESIZE)
        {
            this->vertical_guideline->invalidate();
            this->horizontal_guideline->invalidate();

            this->catchStatus = FIXEDLENGTH;
            this->setAction(ElementType::LINE);
            this->changeLength(this->gui->getLineEditText().toFloat());
        }
        break;

    case Qt::Key_Escape:
        if (this->status == CHANGESIZE)
        {

            this->setAction(ElementType::LINE);
            this->gui->setLineEditText(QString::number(len));
        }
        else
        {
            this->data->deHighlightAll();
            this->vertical_guideline->invalidate();
            this->horizontal_guideline->invalidate();
            this->catchStatus = CLASSIC;
        }
        break;

    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    case Qt::Key_Back:
    case Qt::Key_B:
        if (this->status == CHANGESIZE)
            this->gui->deleteLastCharOfLineEdit();
        break;

    case Qt::Key_Period:
    case Qt::Key_Comma:
        if (this->status == CHANGESIZE)
        {
            if (this->gui->getLineEditText().contains("."))
                break;

            this->gui->appendTextToLineEdit(".");
        }
        break;
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
    float x = this->translateX(event->x());
    float y = this->translateY(event->y());
    this->mouse_end_position.setLocation(event->x(), event->y());

    if (!(event->buttons() & Qt::LeftButton ||
          event->buttons() & Qt::RightButton))
        this->mouse_start_position.setLocation(event->x(), event->y());

    // Right button
    if (event->buttons() & Qt::RightButton ||
        (event->buttons() & Qt::LeftButton &&
         this->status == PAN))
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

    // Left button
    else
    {
        // Some element is being modified, change its size
        if (!this->metaElement.isEmpty())
        {
            switch (this->status)
            {
            case DRAW:
                this->mouseMoveDraw(&x, &y);
                break;

            case SELECT_E:
                this->mouseMoveDraw(&x, &y);
                break;

            default:
                break;
            }

            if (this->catchStatus == FIXEDLENGTH)
            {
                float len = this->gui->getLineEditText().toFloat();
                // Resizes element (change direction)
                this->metaElement.resizeTo(x,y);
                this->changeLength(len);
            }
            else if (this->status != CHANGESIZE)
            {
                // Resizes element
                this->metaElement.resizeTo(x,y);

                this->gui->setLineEditText(QString::number(this->metaElement.getOrigin().distance(x,y)));
            }
        }

        // There is not any element being modified or drawn
        else
        {
            switch(this->status)
            {
            case DRAW:
                catchToClosePoint(&x,&y);
                break;

            case SELECT_E:
                if (event->buttons() & Qt::LeftButton)
                {
                    if (this->selection->isDragged())
                        this->selection->drag(x, y);

                    else
                    {
                        this->selection->resize(this->mouse_start_position.getX(),
                                               this->mouse_start_position.getY(),
                                               this->mouse_end_position.getX(),
                                               this->mouse_end_position.getY(),
                                               this->offset,
                                               this->scale);

                        // Select or deselect element
                        foreach (Element *e, this->data->getElements())
                        {
                            if (this->selection->selectionIntersects(e))
                                e->selectMe();

                            else
                                e->deSelectMe();
                        }
                    }
                }
                break;

            default:
                break;
            }
        }
    }

    // Repaint scene
    this->repaint();
}

/**
 * @brief Change line's direction and set specified length
 * @param k New direction
 * @param len Line's length
 * @param x1 Line's first coordinate x
 * @param y1 Line's first coordinate y
 * @param x2 Line's second coordinate x
 * @param y2 Line's second coordinate y
 */
void OpenGLWidget::changeLengthNotMoveMouse(float len, float x1, float y1, float *x2, float *y2)
{
    if (len < 1)
        return;

    // Straight lines
    if (y1 == *y2)
    {
        if (*x2 >= x1)
            *x2 = x1 + len;
        else *x2 = x1 - len;
        return;
    }
    else if (x1 == *x2)
    {
        if (*y2 >= y1)
            *y2 = y1 + len;
        else *y2 = y1 - len;
        return;
    }

    // Direction
    float k = fabs((*y2 - y1) / (*x2 - x1));

    // count dinstance for x2,y2 from start point
    float dx = sqrt(pow(len,2.) / (pow(k,2.) + 1));
    float dy = k * dx;

    // 1st quadrant
    if ((*x2 > x1) && (*y2 >= y1))
    {
        *x2 = x1 + dx;
        *y2 = y1 + dy;
    }

    // 2nd quadrant
    else if ((*x2 <= x1) && (*y2 > y1))
    {
        *x2 = x1 - dx;
        *y2 = y1 + dy;
    }

    // 3rd quadrant
    else if ((*x2 < x1) && (*y2 < y1))
    {
        *x2 = x1 - dx;
        *y2 = y1 - dy;
    }

    // 4th quadrant
    else if ((*x2 > x1) && (*y2 < y1))
    {
        *x2 = x1 + dx;
        *y2 = y1 - dy;
    }
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

    bool perpendicularFound = false;

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
            this->metaElement.highlightMe();
            e->highlightMe();
            perpendicularFound = true;

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

    if (!perpendicularFound)
    {
        this->metaElement.deHighlightMe();
        this->data->deHighlightAll();
    }
    return perpendicularFound;
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

// NEFUNGUJE - JEN NA VODOROVNY CARY
// TODO: PREPSAT!!
void OpenGLWidget::catchToMiddleOfLine(float x11, float y11, float *x, float *y)
{
    bool middleFound = false;

    // Find if some line is perpendicular to current painted line
    foreach (Element *element, this->data->getElements())
    {
        Line *e = (Line *)element;

        // Get lines coordinates
        float x1 = e->getP1().getX();
        float y1 = e->getP1().getY();
        float x2 = e->getP2().getX();
        float y2 = e->getP2().getY();

        if (!(((x1 == x11) && (y1 == y11))
                || ((x2 == x11) && (y2 == y11))))
            continue;

        // Get coordinates of middle of line
        float sx = x1 + (x2 - x1) / 2;
        float sy = y1 + (y2 - y1) / 2;

        // Direction from middle of line to point
        float k1 = (*y - sy) / (*x - sx);

        // Direction of line
        float k2 = (y2 - y1) / (x2 - x1);
        //qDebug() << "TEST";
        if (fabs(k1 + (1/k2)) < 0.5)
        {
            //qDebug() << "MIDDLE";
            this->metaElement.highlightMe();
            e->highlightMe();

            if (k1 > 1)
            {
                *x = (-k2 * (*y - sy)) + sx;
            }
            else
            {
                *y = ((*x - sx) / -k2) + sy;
            }
        }
    }
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

    float k;
    float factor = *x - this->metaElement.getOrigin().getX();
    if (fabs(factor) > 1)
        k = ((*y - this->metaElement.getOrigin().getY())
             / (factor));
    else k = INF;

    // Find if some line is parallel to current painted line
    for (; it != allElements.end(); ++it) {
        // Get line
        Line *e = (Line *)(*it);

        // Get lines coordinates
        float x1 = e->getP1().getX();
        float y1 = e->getP1().getY();
        float x2 = e->getP2().getX();
        float y2 = e->getP2().getY();


        // Close to line's first x coordinate
        if (fabs(x1 - *x) < this->treshold_value)
        {
            // No line currently painting
            if ((this->metaElement.isEmpty())
                    || (k == INF)
                    || (this->catchStatus == CLASSIC))
            {
                *x = x1;
            }
            // Change currently painted lines leingth
            else
            {
                float dx = (x1 - *x);
                float dy = k*dx;

                *x = x1;
                *y += dy;
            }

            this->vertical_guideline->set(*x, *y, x1, y1);
            closeXPointFound = true;
        }
        // Close to line's second x coordinate
        else if (fabs(x2 - *x) < this->treshold_value)
        {
            // No line currently painting
            if ((this->metaElement.isEmpty())
                    || (k == INF)
                    || (this->catchStatus == CLASSIC))
            {
                *x = x2;
            }
            // Change currently painted lines leingth
            else
            {
                float dx = (x2 - *x);
                float dy = k*dx;

                *x = x2;
                *y += dy;
            }
            this->vertical_guideline->set(*x, *y, x2, y2);
            closeXPointFound = true;
        }

        if (fabs(y1 - *y) < this->treshold_value)
        {
            // No line currently painting
            if ((this->metaElement.isEmpty())
                    || (k == 0)
                    || (vertical_guideline->isValid())
                    || (this->catchStatus == CLASSIC))
            {
                *y = y1;
                vertical_guideline->resizeTo(*x, *y);
                if (this->catchStatus != CLASSIC)
                {
                    this->data->deHighlightAll();
                    this->metaElement.deHighlightMe();
                }
            }
            // Change currently painted lines leingth
            else
            {
                float dy = (y1 - *y);
                float dx = dy/k;

                *y = y1;
                *x += dx;
            }
            this->horizontal_guideline->set(*x, *y, x1, y1);
            closeYPointFound = true;
        }
        else if (fabs(y2 - *y) < this->treshold_value)
        {
            // No line currently painting
            if ((this->metaElement.isEmpty())
                    || (k == 0)
                    || (vertical_guideline->isValid())
                    || (this->catchStatus == CLASSIC))
            {
                *y = y2;
                vertical_guideline->resizeTo(*x, *y);
                if (this->catchStatus != CLASSIC)
                {
                    this->data->deHighlightAll();
                    this->metaElement.deHighlightMe();
                }
            }
            // Change currently painted lines leingth
            else
            {
                float dy = (y2 - *y);
                float dx = dy/k;

                *y = y2;
                *x += dx;
            }
            this->horizontal_guideline->set(*x,*y, x2,y2);
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
    this->selection->deactivate();
    this->metaElement.clear();

    switch (this->status)
    {
    case PAN:
        this->setCursor(Qt::PointingHandCursor);
        break;

    case ROTATE:
        this->setCursor(Qt::SizeBDiagCursor);
        break;

    default:
        this->setCursor(Qt::ArrowCursor);
    }
}

/**
 * @brief Deletes selected items from the data structure
 */
void OpenGLWidget::deleteSelection()
{
    // Delete selected items from data
    foreach (Element *e, this->selection->getSelectedItems())
        this->data->remove(e);

    // Logs deletion
    ChangesLog::sharedInstance()->doStep(DELETE,0,0,new std::vector<Element *>(this->selection->getSelectedItems()));

    // Disables redo and enables undo
    this->gui->setUndo(true);
    this->gui->setRedo(false);

    // Clear selection
    this->selection->clear();

    // Repaint scene
    this->repaint();
}

/**
 * @brief Change length of currently painted or selected line
 * @param new length
 */
void OpenGLWidget::changeLength(float length)
{
    if (length < 1)
        return;

    Line *l = NULL;
    if (this->status != DRAW && this->type != ElementType::LINE)
        if (!this->selection->empty())
            l = (Line *)(this->selection->front());
        else return;
    else
        l = (Line *)this->metaElement.getElement();

    float x1 = l->getP1().getX();
    float y1 = l->getP1().getY();
    float x2 = l->getP2().getX();
    float y2 = l->getP2().getY();

    // Straight lines
    if (y1 == y2)
    {
        if (x2 >= x1)
            x2 = x1 + length;
        else x2 = x1 - length;

        l->resize(x1,y1,x2,y2);
        return;
    }
    else if (x1 == x2)
    {
        if (y2 >= y1)
            y2 = y1 + length;
        else y2 = y1 - length;

        l->resize(x1,y1,x2,y2);
        return;
    }

    // Direction
    float k = fabs((y2 - y1) / (x2 - x1));

    // Origin length
    float oldLength = sqrt(pow(x2 - x1,2.) + pow(y2 - y1,2.));

    // Set the same length
    if (fabs(oldLength - length) == 0)
        return;

    float dx = sqrt(pow(length,2.) / (pow(k,2.) + 1));
    float dy = k * dx;

    // 1st quadrant
    if ((x2 > x1) && (y2 >= y1))
    {
        x2 = x1 + dx;
        y2 = y1 + dy;
    }

    // 2nd quadrant
    else if ((x2 <= x1) && (y2 > y1))
    {
        x2 = x1 - dx;
        y2 = y1 + dy;
    }

    // 3rd quadrant
    else if ((x2 < x1) && (y2 < y1))
    {
        x2 = x1 - dx;
        y2 = y1 - dy;
    }

    // 4th quadrant
    else if ((x2 > x1) && (y2 < y1))
    {
        x2 = x1 + dx;
        y2 = y1 - dy;
    }

    l->resize(x1,y1,x2,y2);

    this->repaint();
}

/**
 * @brief Returns first element at the mouse position
 * @return Reference to first element under cursor
 */
Element * OpenGLWidget::topObjectAtMousePosition()
{
    float x = this->translateX(this->mouse_end_position.getX());
    float y = this->translateY(this->mouse_end_position.getY());

    // Checks selected items first
    for (std::vector<Element*>::reverse_iterator it = this->selection->getSelectedItems().rbegin();
         it != this->selection->getSelectedItems().rend();
         ++it)
    {
        Element *e = (Element*) *it;
        if (e->intersects(x, y))
            return e;
    }

    // Checks all data
    for (std::vector<Element*>::reverse_iterator it = this->data->getElements().rbegin();
         it != data->getElements().rend();
         ++it)
    {
        Element *e = (Element*) *it;
        if (e->intersects(x, y))
            return e;
    }

    return NULL;
}

/**
 * @brief Implements selection or deselection of elements when mouse is released and select tool selected
 */
void OpenGLWidget::mouseReleaseSelect()
{
    if  (this->selection->isActive())
    {
        // Selects all elements within selection rectangle
        foreach (Element *e, this->data->getElements())
        {
            if (this->selection->selectionIntersects(e))
            {
                this->selection->addBack(e);
            }
            else
                e->deSelectMe();
        }

        // Deactivates selection rectangle
        this->selection->deactivate();
    }

    else if (this->metaElement.getElement() == this->selection)
    {
        foreach (Element *e, this->selection->getSelectedItems())
            this->data->add(e);
        this->metaElement.clear();
        this->selection->finalizeResize();

        // Dehighlight all elements
        this->data->deHighlightAll();

        // Clear dotted lines
        this->vertical_guideline->invalidate();
        this->horizontal_guideline->invalidate();
    }

    else if (this->selection->isDragged())
    {
        this->selection->finishDragging(this->translateX(this->mouse_end_position.getX()),
                                        this->translateY(this->mouse_end_position.getY()));
        this->repaint();
    }
}

/**
 * @brief Implements
 */
void OpenGLWidget::mousePressSelect()
{
    float origin_x;
    float origin_y;

    if (this->selection->getCounterPointAndCalculatePoints(this->translateX(this->mouse_end_position.getX()),
                                                           this->translateY(this->mouse_end_position.getY()),
                                                           &origin_x,
                                                           &origin_y))
    {
        // Sets selection as an element being resized
        this->metaElement.set(this->selection, origin_x, origin_y);

        // Removes elements from data structure
        foreach (Element *e, this->selection->getSelectedItems())
            this->data->remove(e);

        return;
    }

    // Gets first object under cursor
    Element * object = this->topObjectAtMousePosition();

    // Some object is under cursor
    if (object == NULL)
    {
        // Clears selected items
        this->selection->clear();

        // Starts displaying selection rectangle
        this->selection->resize(this->mouse_start_position.getX(),
                                this->mouse_start_position.getY(),
                                this->mouse_end_position.getX(),
                                this->mouse_end_position.getY(),
                                this->offset,
                                this->scale);
    }
    else
    {
        // Selection contains object, move objects
        if (selection->contains(object))
            this->selection->startDragging(this->translateX(this->mouse_start_position.getX()),
                                           this->translateY(this->mouse_start_position.getY()));

        // Selection does not contains object under cursor,
        // deselect all objects and select the new one
        else
        {
            this->selection->clear();
            this->selection->addBack(object);
            this->selection->deactivate();
        }
    }
}

void OpenGLWidget::setAction(ElementType::Type t)
{
    this->status = DRAW;
    this->type = t;
}

void OpenGLWidget::mouseMoveDraw(float *x, float *y)
{
    // Save lines origin point
    float x1 = this->metaElement.getOrigin().getX();
    float y1 = this->metaElement.getOrigin().getY();

    // Catch only to x and y axes
    // Don't change line's length
    if (this->catchStatus == FIXEDLENGTH)
    {
        if (isHorizontal(y1, *y))
        {
            float len = sqrt(pow(*x-x1,2.) + pow(*y-y1,2.));
            *y = y1;
            if (*x > x1)
                *x = x1 + len;
            else *x = x1 - len;
        }
        else if (isVertical(x1, *x))
        {
            float len = sqrt(pow(*x-x1,2.) + pow(*y-y1,2.));
            *x = x1;
            if (*y > y1)
                *y = y1 + len;
            else *y = y1 - len;
        }
    }
    // Catch to point / parallel line / perendicular line
    // May change line's length
    else
    {
        if (this->isHorizontal(y1,*y))
            *y = y1;
        // Check if painted line is almost vertical
        else if (this->isVertical(x1,*x))
            *x = x1;
        // Check if painted line is almost diagonal
        else (this->catchToDiagonal(x, y, x1, y1));

        switch (this->catchStatus)
        {
        case PARALLEL:
            catchToParallelLine(x1,y1,x,y);
            break;
        case PERPENDICULAR:
            catchToPerpendicular(x1,y1,x,y);
            break;
        case MIDDLE:
            catchToMiddleOfLine(x1,y1,x,y);
            break;
        case FIXEDLENGTH:
            break;
        }

        // Try to catch point to another very close point
        catchToClosePoint(x,y);
    }
}

/**
 * @brief Translates x position of the mouse into x position in model coordinate system
 * @param x Coordinate x of the mouse in window
 * @return Adequate coordinate x in model
 */
float OpenGLWidget::translateX(float x) const
{
    return (x / this->scale) - this->offset.getX();
}

/**
 * @brief Translates y position of the mouse into y position in model coordinate system
 * @param y Coordinate y of the mouse in window
 * @return Adequate coordinate y in model
 */
float OpenGLWidget::translateY(float y) const
{
    return (y / this->scale) - this->offset.getY();
}

void OpenGLWidget::paintRuler()
{
    // Clears space for ruler
    glColor3f(1,1,1);
    glRectf(0,0,this->width(),20);
    glRectf(0,20,20,this->height());

    // Draws mouse difference on horizontal ruler
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.5, 0.75, 0.95, 0.3);
    if (this->mouse_start_position.getX() < this->mouse_end_position.getX())
        glRectf(this->mouse_start_position.getX(),
                0,
                this->mouse_end_position.getX(),
                20);
    else
        glRectf(this->mouse_end_position.getX(),
                0,
                this->mouse_start_position.getX(),
                20);

    // Draws mouse difference on vertical ruler
    if (this->mouse_start_position.getY() < this->mouse_end_position.getY())
        glRectf(0,
                this->mouse_start_position.getY(),
                20,
                this->mouse_end_position.getY());
    else
        glRectf(0,
                this->mouse_end_position.getY(),
                20,
                this->mouse_start_position.getY());
    glDisable(GL_BLEND);

    // Draws mouse position on both rulers
    glColor3f(0,0,0);
    this->drawLine(this->mouse_end_position.getX(), 0, this->mouse_end_position.getX(), 20);
    this->drawLine(0, this->mouse_end_position.getY(), 20, this->mouse_end_position.getY());

    // Draws ruler boundaries
    glColor3f(0.8, 0.8, 0.8);
    this->drawRectangle(20, 0, this->width(), 20);
    this->drawRectangle(0, 20, 20, this->height());

    // Magic
    int numbers = (int)(this->scale * 100);
    float i1 = 1;

    // Determines how devision ratio
    while (numbers < 50)  {numbers *= 2; i1*=2;}
    while (numbers > 100) {numbers /= 2; i1/=2;}

    // Creates scale size
    float normalized_n = i1 * 100;

    // Determines first x coordinate
    int x = (int) (this->offset.getX()) % (int)(normalized_n);
    int y = (int) (this->offset.getY()) % (int)(normalized_n);

    // Normalizes x counter
    float j = (int)(-this->offset.getX() / normalized_n) * normalized_n;

    // Draws x ruler
    for (int i = x; i < this->width()/this->scale; i+=normalized_n, j+=normalized_n)
    {
        float tmp_x = i*this->scale;
        this->drawLine(tmp_x, 0, tmp_x, 20);
        // Draw number
        QString str = QString::number(j);
        font->render(str.toStdString().c_str(), tmp_x + 2, 10);
    }

    // Normalizes y counter
    j = (int)(-this->offset.getY() / normalized_n) * normalized_n;

    // Draws y ruler
    for (int i = y; i < this->height()/this->scale; i+=normalized_n, j+=normalized_n)
    {
        float tmp_y = i*this->scale;
        this->drawLine(0, tmp_y, 20, tmp_y);
        // Draw number
        QString str = QString::number(j);
        font->render(str.toStdString().c_str(), 0, tmp_y + 2);
    }

    // Repaints 20x20 rectangle at the beginning
    glColor3f(1, 1, 1);
    glRectf(0, 0, 19, 20);
}

void OpenGLWidget::drawRectangle(float x1, float y1, float x2, float y2) const
{
    this->drawLine(x1, y1, x2, y1);
    this->drawLine(x2, y1, x2, y2);
    this->drawLine(x2, y2, x1, y2);
    this->drawLine(x1, y2, x1, y1);
}

void OpenGLWidget::drawLine(float x1, float y1, float x2, float y2) const
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}





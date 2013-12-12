/**
 * PGR 2013 project
 * Selection rectangle implementation
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/29
 * @version 1
 * @file    selectionrectangle.cpp
 */
#include "selectionrectangle.h"
#include "changeslog.h"

/**
 * @brief SelectionRectangle constructor
 */
SelectionRectangle::SelectionRectangle(Gui *gui)
    : min_x(0)
    , min_y(0)
    , max_x(0)
    , max_y(0)
    , active (false)
    , gui(gui)
    , bounding_rectangle(new Rectangle(0, 0, 0, 0))
    , valid_bounding_rectangle(false)
    , orientation(Qt::TopLeftCorner)
    , dragging_items(false)
{
}

/**
 * @brief Selection rectangle virtual destructor
 */
SelectionRectangle::~SelectionRectangle()
{
    delete bounding_rectangle;
}

/**
 * @brief Paints slection rectangle
 */
void SelectionRectangle::paintMe() const
{
    // Selection is not active
    if (this->active)
    {
        // Enables blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Setst color
        glColor4f(0.5, 0.75, 0.95, 0.7);

        // Fill's rectangle
        glBegin(GL_QUADS);
        glVertex2f(this->min_x, this->min_y);
        glVertex2f(this->max_x, this->min_y);
        glVertex2f(this->max_x, this->max_y);
        glVertex2f(this->min_x, this->max_y);
        glEnd();

        // Disables blending
        glDisable(GL_BLEND);

        // Draws rectangle's borders
        glBegin(GL_LINES);
        // Top
        glVertex2f(this->min_x, this->min_y);
        glVertex2f(this->max_x, this->min_y);
        // Right
        glVertex2f(this->max_x, this->min_y);
        glVertex2f(this->max_x, this->max_y);
        // Bottom
        glVertex2f(this->max_x, this->max_y);
        glVertex2f(this->min_x, this->max_y);
        // Left
        glVertex2f(this->min_x, this->max_y);
        glVertex2f(this->min_x, this->min_y);
        glEnd();
    }
}

/**
 * @brief Does not have any meaning, just required redefinishion...
 */
void SelectionRectangle::paintPoints() const
{
}

/**
 * @brief Paints bounding rectangle
 * @param x
 * @param y
 */
void SelectionRectangle::paintBoundingRectangle(float x, float y) const
{
    if (this->valid_bounding_rectangle)
    {
        // Saves matrix
        glPushMatrix();

        // Translates scene by offset
        glTranslatef(this->offset_x,
                     this->offset_y,
                     0);
        Point::paintPoint(this->centre_of_rotation, x, y);
        Point::paintPoint(this->centre_of_rotation.getX() + this->rotation_point.getX(),
                          this->centre_of_rotation.getY() + this->rotation_point.getY(),
                          x,
                          y);

        glTranslatef(this->centre_of_rotation.getX(),
                     this->centre_of_rotation.getY(),
                     0);
        glRotatef(this->rotation,
                  0,
                  0,
                  1.0);
        glTranslatef(-this->centre_of_rotation.getX(),
                     -this->centre_of_rotation.getY(),
                     0);
        this->bounding_rectangle->paintMe();
        this->bounding_rectangle->paintPoints(x,y);
        glTranslatef(this->centre_of_rotation.getX(),
                     this->centre_of_rotation.getY(),
                     0);

        glTranslatef(-this->centre_of_rotation.getX(),
                     -this->centre_of_rotation.getY(),
                     0);
        foreach (Element *e, this->selected_items)
            e->paintMe();

        glPopMatrix();
    }
}

/**
 * @brief Resizes and activates selection rectangle from diagonal
 * @param x1 Coordinate x of first point
 * @param y1 Coordinate y of first point
 * @param x2 Coordinate x of second point
 * @param y2 Coordinate y of second point
 */
void SelectionRectangle::resize(float x1, float y1, float x2, float y2, const Point offset, float scale)
{
    this->width = fabs(x1 - x2);
    this->height = fabs(y1 - y2);

    this->min_x = x1 < x2 ? x1 : x2;
    this->min_y = y1 < y2 ? y1 : y2;
    this->max_x = min_x + this->width;
    this->max_y = min_y + this->height;

    this->active = true;

    this->transform(offset, scale);
}

void SelectionRectangle::resize(float x1, float y1, float x2, float y2, Qt::Corner orientation)
{
    this->orientation = orientation;
//    this->resize(x1,y1,x2,y2);
}

void SelectionRectangle::resize(float x, float y)
{
    float scale_x, scale_y;
    if (this->minx == this->maxx)
    {
        scale_x = 0;
        scale_y = (this->origin.getY() - y)/(this->maxy -this->miny);
        this->bounding_rectangle->resize(this->origin.getX(),
                                         this->origin.getY(),
                                         this->origin.getX()
                                         ,y);
    }
    else if (this->miny == this->maxy)
    {
        scale_x = (this->origin.getX() - x)/(this->maxx -this->minx);
        scale_y = 0;
        this->bounding_rectangle->resize(this->origin.getX(),
                                         this->origin.getY(),
                                         x,
                                         this->origin.getY());
    }
    else
    {
        scale_x = (this->origin.getX() - x)/(this->maxx -this->minx);
        scale_y = (this->origin.getY() - y)/(this->maxy -this->miny);
        this->bounding_rectangle->resize(this->origin.getX(),
                                         this->origin.getY(),
                                         x,
                                         y);
    }

    Element *e = NULL;

    float centre_x;
    float centre_y;

    switch(this->orientation)
    {
    case Qt::TopLeftCorner:

        for (unsigned long int i = 0; i < this->selected_items.size()*4; i+=4)
        {
            e = this->selected_items.at(i/4);
            e->resizeToBoundingRectangle(this->maxx - this->list_of_points[i]   * scale_x,
                                         this->maxy - this->list_of_points[i+1] * scale_y,
                                         this->maxx - this->list_of_points[i+2] * scale_x,
                                         this->maxy - this->list_of_points[i+3] * scale_y);
        }

        centre_x = x + (this->origin.getX() - x)/2;
        centre_y = y + (this->origin.getY() - y)/2;
        break;

    case Qt::BottomRightCorner:
        for (unsigned long int i = 0; i < this->selected_items.size()*4; i+=4)
        {
            e = this->selected_items.at(i/4);
            e->resizeToBoundingRectangle(this->minx - this->list_of_points[i]   * scale_x,
                                         this->miny - this->list_of_points[i+1] * scale_y,
                                         this->minx - this->list_of_points[i+2] * scale_x,
                                         this->miny - this->list_of_points[i+3] * scale_y);
        }

        centre_x = this->origin.getX() + (x - this->origin.getX())/2;
        centre_y = this->origin.getY() + (y - this->origin.getY())/2;
        break;

    case Qt::TopRightCorner:
        for (unsigned long int i = 0; i < this->selected_items.size()*4; i+=4)
        {
            e = this->selected_items.at(i/4);
            e->resizeToBoundingRectangle(this->minx - this->list_of_points[i]   * scale_x,
                                         this->maxy - this->list_of_points[i+1] * scale_y,
                                         this->minx - this->list_of_points[i+2] * scale_x,
                                         this->maxy - this->list_of_points[i+3] * scale_y);
        }

        centre_x = this->origin.getX() + (x - this->origin.getX())/2;
        centre_y = y + (this->origin.getY() - y)/2;
        break;

    case Qt::BottomLeftCorner:

        for (unsigned long int i = 0; i < this->selected_items.size()*4; i+=4)
        {
            e = this->selected_items.at(i/4);
            e->resizeToBoundingRectangle(this->maxx - this->list_of_points[i]   * scale_x,
                                         this->miny - this->list_of_points[i+1] * scale_y,
                                         this->maxx - this->list_of_points[i+2] * scale_x,
                                         this->miny - this->list_of_points[i+3] * scale_y);
        }

        centre_x = x + (this->origin.getX() - x)/2;
        centre_y = this->origin.getY() + (y - this->origin.getY())/2;
        break;

    }

    this->centre_of_rotation.setLocation(centre_x, centre_y);
}

/**
 * @brief Deactivates selection rectangle
 */
void SelectionRectangle::deactivate()
{
    this->active = false;
    this->calculateBoundingRectangle();

    this->centre_of_rotation.setLocation(this->minx + (this->maxx - this->minx)/2,
                                         this->miny + (this->maxy - this->miny)/2);
}

/**
 * @brief Tests if rectangle intersects with point
 * @param point Point to intersect with
 * @return True if point and rectangle intersects
 */
bool SelectionRectangle::selectionIntersects(Point point) const
{
    return this->selectionIntersects(point.getX(), point.getY());
}

/**
 * @brief Tests if rectangle intersects with point
 * @param x Point's coordinate x
 * @param y Point's coordinate y
 * @return True if point and rectangle intersects
 */
bool SelectionRectangle::selectionIntersects(float x, float y) const
{
    return this->translated_min_x < x &&
           this->translated_max_x > x &&
           this->translated_min_y < y &&
           this->translated_max_y > y;
}

/**
 * @brief Transforms rectangle to match model's offset
 * @param offset Offset of the scene
 * @param scale Scale of the scene
 */
void SelectionRectangle::transform(Point offset, float scale)
{
    this->translated_min_x = (this->min_x / scale) - offset.getX();
    this->translated_max_x = (this->max_x / scale) - offset.getX();
    this->translated_min_y = (this->min_y / scale) - offset.getY();
    this->translated_max_y = (this->max_y / scale) - offset.getY();
}

bool SelectionRectangle::selectionIntersects(const Element *e) const
{
    return e->intersects(this->translated_min_x,
                         this->translated_min_y,
                         this->translated_max_x,
                         this->translated_max_y);
}

bool SelectionRectangle::isActive() const
{
    return this->active;
}

bool SelectionRectangle::empty() const
{
    return this->selected_items.empty();
}

Element * SelectionRectangle::front() const
{
    return this->selected_items.front();
}

std::vector<Element *> SelectionRectangle::getSelectedItems() const
{
    return this->selected_items;
}

void SelectionRectangle::clear()
{
    this->deselectAll();
    this->valid_bounding_rectangle = false;
    this->selected_items.clear();
    this->gui->setDelete(false);
}

void SelectionRectangle::addBack(Element *e)
{
    e->selectMe();
    this->selected_items.push_back(e);
    this->gui->setDelete(true);
}

void SelectionRectangle::deselectAll()
{
    foreach (Element *e, this->selected_items)
        e->deSelectMe();
}

/**
 * @brief Tests if selection contains element
 * @param object Element to be found in selected items
 * @return True if element is found in selected items
 */
bool SelectionRectangle::contains(Element *object) const
{
    foreach(Element* e, this->selected_items)
    {
        if (e == object)
            return true;
    }

    return false;
}

void SelectionRectangle::calculateBoundingRectangle()
{
    if (this->selected_items.empty())
        this->valid_bounding_rectangle = false;

    else
    {
        this->valid_bounding_rectangle = true;

        Element *e = this->selected_items.front();

        this->minx = e->getMinX();
        this->miny = e->getMinY();
        this->maxx = e->getMaxX();
        this->maxy = e->getMaxY();

        foreach (e, this->selected_items)
        {
            float t_minx = e->getMinX();
            float t_miny = e->getMinY();
            float t_maxx = e->getMaxX();
            float t_maxy = e->getMaxY();

            this->minx = this->minx < t_minx ? this->minx : t_minx;
            this->miny = this->miny < t_miny ? this->miny : t_miny;
            this->maxx = this->maxx > t_maxx ? this->maxx : t_maxx;
            this->maxy = this->maxy > t_maxy ? this->maxy : t_maxy;
        }

        this->bounding_rectangle->resize(this->minx,
                                         this->miny,
                                         this->maxx,
                                         this->maxy);

        this->rotation_point.setLocation(3 * OpenGLWidget::treshold_value, 0);
    }
}

bool SelectionRectangle::getCounterPointAndCalculatePoints(float x, float y, float *ox, float *oy)
{
    if (this->bounding_rectangle->getCounterPoint(x,y,ox,oy, &this->orientation))
    {
        this->list_of_points.clear();

        this->storeDistancesToFixedPoint();

        return true;
    }
    return false;
}

void SelectionRectangle::storeDistancesToFixedPoint()
{
    this->list_of_points.clear();


    switch(this->orientation)
    {
    case Qt::TopLeftCorner:
        foreach (Element *e, this->selected_items)
        {
            this->list_of_points.push_back(this->maxx - e->getMinX());
            this->list_of_points.push_back(this->maxy - e->getMinY());
            this->list_of_points.push_back(this->maxx - e->getMaxX());
            this->list_of_points.push_back(this->maxy - e->getMaxY());
        }
        break;

    case Qt::BottomRightCorner:
        foreach (Element *e, this->selected_items)
        {
            this->list_of_points.push_back(e->getMinX() - this->minx);
            this->list_of_points.push_back(e->getMinY() - this->miny);
            this->list_of_points.push_back(e->getMaxX() - this->minx);
            this->list_of_points.push_back(e->getMaxY() - this->miny);
        }
        break;

    case Qt::TopRightCorner:
        foreach (Element *e, this->selected_items)
        {
            this->list_of_points.push_back(e->getMinX() - this->minx);
            this->list_of_points.push_back(this->maxy - e->getMinY());
            this->list_of_points.push_back(e->getMaxX() - this->minx);
            this->list_of_points.push_back(this->maxy - e->getMaxY());
        }
        break;

    case Qt::BottomLeftCorner:
        foreach (Element *e, this->selected_items)
        {
            this->list_of_points.push_back(this->maxx - e->getMinX());
            this->list_of_points.push_back(e->getMinY() - this->miny);
            this->list_of_points.push_back(this->maxx - e->getMaxX());
            this->list_of_points.push_back(e->getMaxY() - this->miny);
        }
        break;
    }
}

void SelectionRectangle::finalizeResize()
{
    foreach (Element *e, this->selected_items)
        e->finalizeResize();

    this->calculateBoundingRectangle();

    this->resized = false;
}

void SelectionRectangle::startDragging(float x, float y)
{
    this->start_x = x;
    this->start_y = y;
    this->dragging_items = true;
}

bool SelectionRectangle::isDragged() const
{
    return this->dragging_items;
}

void SelectionRectangle::finishDragging(float x, float y)
{
    this->offset_x = 0;
    this->offset_y = 0;

    this->dragging_items = false;

    float dx = x - this->start_x;
    float dy = y - this->start_y;

    foreach (Element *e, this->selected_items)
        e->translatef(dx, dy);

    ChangesLog::sharedInstance()->doStep(MOVE, dx, dy, new std::vector<Element *>(this->selected_items));

    this->calculateBoundingRectangle();

    this->centre_of_rotation.setLocation(this->centre_of_rotation.getX() + dx,
                                         this->centre_of_rotation.getY() + dy);
}

void SelectionRectangle::drag(float x, float y)
{
    this->offset_x = x - this->start_x;
    this->offset_y = y - this->start_y;
}

Qt::Corner SelectionRectangle::getOrientation()
{
    return this->orientation;
}

bool SelectionRectangle::isCentreOfRotation(float x, float y)
{
    return this->centre_of_rotation.isNearby(x,y);
}

bool SelectionRectangle::isRotationPoint(float x, float y)
{
    return Point::isNearby(this->centre_of_rotation.getX() + this->rotation_point.getX(),
                           this->centre_of_rotation.getY() + this->rotation_point.getY(),
                           x,
                           y);
}

bool SelectionRectangle::isCentreOfRotationDragged()
{
    return this->centre_of_origin_dragged;
}

bool SelectionRectangle::isRotationPointDragged()
{
    return this->rotation_point_dragged;
}

void SelectionRectangle::dragCentreOfRotation(float x, float y)
{
    this->centre_of_rotation.setLocation(x,y);
}

void SelectionRectangle::dragRotationPoint(float x, float y)
{
    float dx = x - this->centre_of_rotation.getX();
    float dy = y - this->centre_of_rotation.getY();
    this->rotation_point.setLocation(dx, dy);

    this->rotation = atanf(dy/dx) * 180 / 3.14;
    if (dx < 0)
        this->rotation += 180;
}

void SelectionRectangle::setCentreOfRotationDragged(bool val)
{
    this->centre_of_origin_dragged = val;
}

void SelectionRectangle::setRotationPointDragged(bool val)
{
    this->rotation_point_dragged = val;

    // Finished rotationg
    if (!this->rotation_point_dragged)
    {
        float rot = this->rotation * 3.14/ 180;

        foreach (Element *e, this->selected_items)
            e->rotate(this->centre_of_rotation, rot);
    }

    this->rotation = 0;
    this->calculateBoundingRectangle();
}

bool SelectionRectangle::isResized()
{
    return this->resized;
}

void SelectionRectangle::startResize(float x, float y)
{
    this->resized = true;
    this->origin.setLocation(x,y);
}

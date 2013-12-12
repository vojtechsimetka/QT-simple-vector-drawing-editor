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
#include "selection.h"
#include "changeslog.h"

/**
 * @brief SelectionRectangle constructor
 */
Selection::Selection(Gui *gui)
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
Selection::~Selection()
{
    delete bounding_rectangle;
}

/**
 * @brief Paints selection rectangle
 */
void Selection::paintMe() const
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
 * @brief Paints bounding rectangle
 * @param x Mouse position coordinate x
 * @param y Mouse position coordinate y
 */
void Selection::paintBoundingRectangle(float x, float y) const
{
    if (this->valid_bounding_rectangle)
    {
        // Saves matrix
        glPushMatrix();

        // Translates scene by offset
        glTranslatef(this->offset_x,
                     this->offset_y,
                     0);

        // Paints rotation centre
        Point::paintPoint(this->centre_of_rotation, x, y);
        Point::paintPoint(this->centre_of_rotation.getX() + this->rotation_point.getX(),
                          this->centre_of_rotation.getY() + this->rotation_point.getY(),
                          x,
                          y);

        // Moves scene to rotation centre
        glTranslatef(this->centre_of_rotation.getX(),
                     this->centre_of_rotation.getY(),
                     0);

        // Rotates scene around z axis
        glRotatef(this->rotation,
                  0,
                  0,
                  1.0);

        // Moves scene back where it was before rotation
        glTranslatef(-this->centre_of_rotation.getX(),
                     -this->centre_of_rotation.getY(),
                     0);

        // Paints bounding rectangle
        this->bounding_rectangle->paintMe();
        this->bounding_rectangle->paintPoints(x,y);

        // Paints all elemenets
        foreach (Element *e, this->selected_items)
            e->forcedPaintMe();

        // Restores matrix
        glPopMatrix();
    }
}

/**
 * @brief Resizes and activates selection rectangle from diagonal
 * @param x1 Coordinate x of first point
 * @param y1 Coordinate y of first point
 * @param x2 Coordinate x of second point
 * @param y2 Coordinate y of second point
 * @param offset Model's offset
 * @param scale Model's scale
 */
void Selection::resizeSelectionRectangle(float x1, float y1, float x2, float y2, const Point offset, float scale)
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

/**
 * @brief Resizes selected items to new bounding box
 * @param ox Origin's coordinate x
 * @param oy Origin's coordinate y
 * @param x Coordinate x of extension
 * @param y Coordinate y of extension
 * @param orientation Orientation of the scale
 */
void Selection::resizeSelectedItems(float ox, float oy, float x, float y, Qt::Corner orientation)
{
    // Sets oriigin's location
    this->origin.setLocation(ox, oy);

    // Sets orientation of resize
    this->orientation = orientation;

    // Performs the resize
    this->resizeSelectedItems(x, y);
}

/**
 * @brief Resizes selected items to new bounding box
 * @param x Coordinate x of extension of the bounding box
 * @param y Coordinate y of extension of the bounding box
 */
void Selection::resizeSelectedItems(float x, float y)
{
    float scale_x, scale_y;

    // Selection has degraded to vertical line
    if (this->minx == this->maxx)
    {
        scale_x = 0;
        scale_y = (this->origin.getY() - y)/(this->maxy -this->miny);
        this->bounding_rectangle->resize(this->origin.getX(),
                                         this->origin.getY(),
                                         this->origin.getX()
                                         ,y);
    }

    // Selection has degraded to horizontal line
    else if (this->miny == this->maxy)
    {
        scale_x = (this->origin.getX() - x)/(this->maxx -this->minx);
        scale_y = 0;
        this->bounding_rectangle->resize(this->origin.getX(),
                                         this->origin.getY(),
                                         x,
                                         this->origin.getY());
    }

    // Selection can be extended both vertically and horizontally
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

    // Based on orientation determines how objects should be extended
    switch(this->orientation)
    {
    case Qt::TopLeftCorner:

        for (unsigned long int i = 0; i < this->selected_items.size()*4; i+=4)
        {
            e = this->selected_items.at(i/4);

            // Resizes one object from fixed point with coordinates maxx and maxy
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

            // Resizes one object from fixed point with coordinates minx and miny
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

            // Resizes one object from fixed point with coordinates minx and maxy
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

            // Resizes one object from fixed point with coordinates maxx and miny
            e->resizeToBoundingRectangle(this->maxx - this->list_of_points[i]   * scale_x,
                                         this->miny - this->list_of_points[i+1] * scale_y,
                                         this->maxx - this->list_of_points[i+2] * scale_x,
                                         this->miny - this->list_of_points[i+3] * scale_y);
        }

        centre_x = x + (this->origin.getX() - x)/2;
        centre_y = this->origin.getY() + (y - this->origin.getY())/2;
        break;

    }

    // Sets resized location centre of rotation
    this->centre_of_rotation.setLocation(centre_x, centre_y);
}

/**
 * @brief Deactivates selection rectangle
 */
void Selection::deactivate()
{
    this->active = false;
    this->calculateBoundingRectangle();

    // Sets centre of ratation to the centre of bounding rectangle
    this->centre_of_rotation.setLocation(this->minx + (this->maxx - this->minx)/2,
                                         this->miny + (this->maxy - this->miny)/2);
}

/**
 * @brief Tests if selection rectangle intersects with point
 * @param point Point to intersect with
 * @return True if point and rectangle intersects
 */
bool Selection::selectionIntersects(Point point) const
{
    return this->selectionIntersects(point.getX(), point.getY());
}

/**
 * @brief Tests if selection rectangle intersects with point
 * @param x Point's coordinate x
 * @param y Point's coordinate y
 * @return True if point and rectangle intersects
 */
bool Selection::selectionIntersects(float x, float y) const
{
    return this->translated_min_x < x &&
           this->translated_max_x > x &&
           this->translated_min_y < y &&
           this->translated_max_y > y;
}

/**
 * @brief Transforms rectangle to match model's offset and scale
 * @param offset Offset of the scene
 * @param scale Scale of the scene
 */
void Selection::transform(Point offset, float scale)
{
    this->translated_min_x = (this->min_x / scale) - offset.getX();
    this->translated_max_x = (this->max_x / scale) - offset.getX();
    this->translated_min_y = (this->min_y / scale) - offset.getY();
    this->translated_max_y = (this->max_y / scale) - offset.getY();
}

/**
 * @brief Tests if selection rectangle intersects with an element
 * @param e Element to be tested for intersection
 * @return True if rectangle and element intersects
 */
bool Selection::selectionIntersects(const Element *e) const
{
    return e->intersects(this->translated_min_x,
                         this->translated_min_y,
                         this->translated_max_x,
                         this->translated_max_y);
}

/**
 * @brief Tests if selection rectangle is active
 * @return True if selection rectangle is active
 */
bool Selection::isActive() const
{
    return this->active;
}

/**
 * @brief Tests if there are any selected items
 * @return True if there aren't any selected items
 */
bool Selection::empty() const
{
    return this->selected_items.empty();
}

/**
 * @brief Returns first element from selected items
 * @return First element from selected items
 */
Element * Selection::front() const
{
    return this->selected_items.front();
}

/**
 * @brief Returns selected items
 * @return Selected items
 */
std::vector<Element *> Selection::getSelectedItems() const
{
    return this->selected_items;
}

/**
 * @brief Clears selected items
 */
void Selection::clear()
{
    // Deselects all items
    this->deselectAll();
    this->valid_bounding_rectangle = false;

    // Clears selected items
    this->selected_items.clear();

    // Disables delete button
    this->gui->setDelete(false);
}

/**
 * @brief Adds new item to the back of selected items
 * @param e Item to be selected
 */
void Selection::addBack(Element *e)
{
    // Selects new item
    e->selectMe();

    // Disables it for drawing
    e->setVisible(false);

    // Stores item to the selection
    this->selected_items.push_back(e);

    // Enables delete button since there is something selected now.
    this->gui->setDelete(true);
}

/**
 * @brief Deselects all items in selection and allows them to be drawn
 */
void Selection::deselectAll()
{
    foreach (Element *e, this->selected_items)
    {
        e->deSelectMe();
        e->setVisible(true);
    }
}

/**
 * @brief Tests if selection contains element
 * @param object Element to be found in selected items
 * @return True if element is found in selected items
 */
bool Selection::contains(Element *object) const
{
    foreach(Element* e, this->selected_items)
    {
        if (e == object)
            return true;
    }

    return false;
}

/**
 * @brief Calculates bounding rectangle
 */
void Selection::calculateBoundingRectangle()
{
    // There aren't any selected items, disables drawing of bounding rectangle
    if (this->selected_items.empty())
        this->valid_bounding_rectangle = false;

    // There are some selected items
    else
    {
        this->valid_bounding_rectangle = true;

        // Determines bounding rectangle size by calculating minimal and maximal coordinate
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

        // Resizes bounding rectangle
        this->bounding_rectangle->resize(this->minx,
                                         this->miny,
                                         this->maxx,
                                         this->maxy);

        // Sets rotation point to be 24 px right to the centre of rotation
        this->rotation_point.setLocation(3 * OpenGLWidget::treshold_value, 0);
    }
}

/**
 * @brief If resizing corner was clicked, stores the opposite point and returns true
 * @param x Mouse position coordinate x
 * @param y Mouse position coordinate y
 * @return True if some corner point was clicked
 */
bool Selection::isResizePointClicked(float x, float y)
{
    float ox, oy;

    // Some corner point was clicked
    if (this->bounding_rectangle->getCounterPoint(x,y,&ox,&oy, &this->orientation))
    {
        this->origin.setLocation(ox, oy);

        return true;
    }
    return false;
}

/**
 * @brief Stores distances to fixed point for each element
 */
void Selection::storeDistancesToFixedPoint()
{
    // Clears distance list
    this->list_of_points.clear();

    // Based on orientation stores distances betwenn element min and max coordinate and the fixed point
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

/**
 * @brief Finishes resize
 */
void Selection::finalizeResize()
{
    // Finishes resize on each element
    foreach (Element *e, this->selected_items)
        e->finalizeResize();

    // Calculates bounding rectangle
    this->calculateBoundingRectangle();

    // Sets resize flag to false
    this->resized = false;
}

/**
 * @brief Starts dragging selected elements
 * @param x Origin point's coordinate x
 * @param y Origin point's coordinate y
 */
void Selection::startDragging(float x, float y)
{
    this->start_x = x;
    this->start_y = y;
    this->dragging_items = true;
}

/**
 * @brief Tests if selected elements are being dragged
 * @return True if selected elements are being dragged
 */
bool Selection::isDragged() const
{
    return this->dragging_items;
}

/**
 * @brief Finishes dragging for all elements
 * @param x Final coordinate x for the origin point
 * @param y Final coordinate y for the origin point
 */
void Selection::finishDragging(float x, float y)
{
    this->offset_x = 0;
    this->offset_y = 0;

    this->dragging_items = false;

    float dx = x - this->start_x;
    float dy = y - this->start_y;

    // Moves all elements by dx
    foreach (Element *e, this->selected_items)
        e->translatef(dx, dy);

    ChangesLog::sharedInstance()->doStep(MOVE, dx, dy, new std::vector<Element *>(this->selected_items));

    // Recalculates bounding rectangle
    this->calculateBoundingRectangle();

    // Moves centre of origin
    this->centre_of_rotation.setLocation(this->centre_of_rotation.getX() + dx,
                                         this->centre_of_rotation.getY() + dy);
}

/**
 * @brief Draggs items
 * @param x Current coordinate x of mouse
 * @param y Current coordinate y of mouse
 */
void Selection::drag(float x, float y)
{
    this->offset_x = x - this->start_x;
    this->offset_y = y - this->start_y;
}

/**
 * @brief Returns orrientation of resize
 * @return Returns orrientation of resize
 */
Qt::Corner Selection::getOrientation()
{
    return this->orientation;
}

/**
 * @brief Tests if point with coordinates x and y lies nearby centre of rotation
 * @param x Coordinate x of mouse position in model coordinates
 * @param y Coordinate y of mouse position in model coordinates
 * @return True if point with coordinates x and y lies nearby centre of rotation
 */
bool Selection::isCentreOfRotation(float x, float y)
{
    return this->centre_of_rotation.isNearby(x,y);
}

/**
 * @brief Tests if point with coordinates x and y lies nearby rotation point
 * @param x Coordinate x of mouse position in model coordinates
 * @param y Coordinate y of mouse position in model coordinates
 * @return True if point with coordinates x and y lies nearby rotation point
 */
bool Selection::isRotationPoint(float x, float y)
{
    return Point::isNearby(this->centre_of_rotation.getX() + this->rotation_point.getX(),
                           this->centre_of_rotation.getY() + this->rotation_point.getY(),
                           x,
                           y);
}

/**
 * @brief Tests if centre of rotation is being dragged
 * @return True if centre of rotation is being dragged
 */
bool Selection::isCentreOfRotationDragged()
{
    return this->centre_of_origin_dragged;
}

/**
 * @brief Tests if rotation point is being dragged
 * @return True if rotation point is being dragged
 */
bool Selection::isRotationPointDragged()
{
    return this->rotation_point_dragged;
}

/**
 * @brief Draggs centre of rotation
 * @param x New coordinate x of centre of rotation
 * @param y New coordinate y of centre of rotation
 */
void Selection::dragCentreOfRotation(float x, float y)
{
    this->centre_of_rotation.setLocation(x,y);
}

/**
 * @brief Draggs rotation pooint
 * @param x New coordinate x of rotation point
 * @param y New coordinate y of rotation point
 */
void Selection::dragRotationPoint(float x, float y)
{
    float dx = x - this->centre_of_rotation.getX();
    float dy = y - this->centre_of_rotation.getY();

    this->rotation_point.setLocation(dx, dy);

    // Calculates rotation
    this->rotation = atanf(dy/dx) * 180 / 3.14;

    // Normalizes the rotation
    if (dx < 0)
        this->rotation += 180;
}

/**
 * @brief Sets if centre of rotation is dragged
 * @param val
 */
void Selection::setCentreOfRotationDragged(bool val)
{
    this->centre_of_origin_dragged = val;
}

/**
 * @brief Sets if rotation point is being dragged, if not rotates the scene
 * @param val
 */
void Selection::setRotationPointDragged(bool val)
{
    this->rotation_point_dragged = val;

    // Finished rotationg
    if (!this->rotation_point_dragged)
    {
        this->rotate(this->centre_of_rotation, this->rotation * 3.14/ 180);

        this->rotation = 0;
    }
}

/**
 * @brief Tests if selected items are being resized
 * @return True if selected items are being resized
 */
bool Selection::isResized()
{
    return this->resized;
}

/**
 * @brief Starts resizing
 */
void Selection::startResize()
{
    this->resized = true;

    // Clears stored points
    this->list_of_points.clear();

    // Stores distance to the fixed point for each element
    this->storeDistancesToFixedPoint();
}

/**
 * @brief Rotates around point by some angle
 * @param cor Centre of rotation
 * @param angle How much should the object be rotated in radians
 */
void Selection::rotate(Point cor, float angle)
{
    foreach (Element *e, this->selected_items)
        e->rotate(cor, angle);

    this->calculateBoundingRectangle();
}

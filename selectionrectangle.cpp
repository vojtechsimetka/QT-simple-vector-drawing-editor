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

/**
 * @brief SelectionRectangle constructor
 */
SelectionRectangle::SelectionRectangle(Gui *gui)
    : Element(ElementType::SELECTION_RECTANGLE)
    , min_x(0)
    , min_y(0)
    , max_x(0)
    , max_y(0)
    , active (false)
    , gui(gui)
    , bounding_rectangle(new Rectangle(0, 0, 0, 0))
    , valid_bounding_rectangle(false)
    , orientation(Qt::TopLeftCorner)
{
}

SelectionRectangle::~SelectionRectangle()
{
    delete bounding_rectangle;
}

/**
 * @brief Paints rectangle
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
    else if (this->valid_bounding_rectangle)
    {
        this->bounding_rectangle->paintMe();
        this->bounding_rectangle->paintPoints();
    }
}

/**
 * @brief Does not have any meaning, just required redefinishion...
 */
void SelectionRectangle::paintPoints() const {}

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

void SelectionRectangle::resize(float ox, float oy, float x, float y)
{
    this->bounding_rectangle->resize(ox,oy,x,y);

    float scale_x, scale_y, t_min_x, t_min_y, t_max_x, t_max_y;
    Element *e = NULL;

    switch(this->orientation)
    {
    case Qt::TopLeftCorner:
        scale_x = (ox - x)/(this->maxx -this->minx);
        scale_y = (oy - y)/(this->maxy -this->miny);

        for (int i = 0; i < this->selected_items.size()*2; i+=2)
        {
            t_min_x = this->list_of_points[i]   * scale_x - this->list_of_points[i];
            t_min_y = this->list_of_points[i+1] * scale_y - this->list_of_points[i+1];

            e = this->selected_items.at(i/2);
            e->setScaleAndTranslation(scale_x,
                                      scale_y,
                                      0,//t_min_x,
                                      0//t_min_y
                                      );

            qDebug() << t_min_x
                     << t_min_y
                     << scale_x
                     << scale_y;
        }

        break;
    }
}

/**
 * @brief Deactivates selection rectangle
 */
void SelectionRectangle::deactivate()
{
    this->active = false;
    this->calculateBoundingRectangle();
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
    }
}

bool SelectionRectangle::intersects(float, float, float, float) const
{
    throw new QString("The function intersects in Selection shouldn't be ever used");
    return false;
}
bool SelectionRectangle::intersects(Point) const
{
    throw new QString("The function intersects in Selection shouldn't be ever used");
    return false;
}
bool SelectionRectangle::getCounterPoint(float x, float y, float *ox, float *oy) const
{
    throw new QString("The function getCounterPoint in Selection shouldn't be ever used");
    return false;
}

bool SelectionRectangle::getCounterPointAndCalculatePoints(float x, float y, float *ox, float *oy)
{
    if (this->bounding_rectangle->getCounterPoint(x,y,ox,oy, &this->orientation))
    {
        this->list_of_points.clear();

        foreach (Element *e, this->selected_items)
        {
            switch(this->orientation)
            {
            case Qt::TopLeftCorner:
                this->list_of_points.push_back(e->getMinX() - this->minx);
                this->list_of_points.push_back(e->getMinY() - this->miny);
            }
        }

        return true;
    }
    return false;
}

float SelectionRectangle::getMinX() const
{

}
float SelectionRectangle::getMinY() const
{

}
float SelectionRectangle::getMaxX() const
{

}
float SelectionRectangle::getMaxY() const
{

}

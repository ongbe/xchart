
#include "graph_element.h"
#include "graph_handle.h"
#include "graph_geometry.h"
#include "igraph_doc.h"

#define HANDLE_SIZE 7

namespace GraphLib
{
    struct Element::PrivateData
    {
        Handle resizeHandles[8];
        Point corner;
        double width;
        double height;

        double extraSpacing;
    };
    Element::Element(int numHandles) : Object()
    {
        m_data = new PrivateData;
        m_data->corner = QPoint(0, 0);
        m_data->width = 10;
        m_data->height = 10;

        for (int i = 0; i < 8; i++) {
            m_data->resizeHandles[i].setType(HANDLE_MAJOR_CONTROL);
        }
    }
    Element::~Element()
    {
        delete m_data;
    }

    Point Element::corner()
    {
        return m_data->corner;
    }
    void Element::setCorner(Point pos)
    {
        m_data->corner = pos;
    }
    double Element::width()
    {
        return m_data->width;
    }
    void Element::setWidth(double width)
    {
        m_data->width = width;
    }
    double Element::height()
    {
        return m_data->height;
    }
    void Element::setHeight(double h)
    {
        m_data->height = h;
    }

    GraphObjectChange *Element::move(Point *to)
    {
        setCorner(*to);
        updateHandles();
        return NULL;
    }


    void Element::updateHandles()
    {
        Point *corner = &m_data->corner;
        m_data->resizeHandles[0].setId(HANDLE_RESIZE_NW);
        m_data->resizeHandles[0].setPos(Point(corner->x() + HANDLE_SIZE/2, 
                    corner->y() + HANDLE_SIZE/2));

        m_data->resizeHandles[1].setId(HANDLE_RESIZE_N);
        m_data->resizeHandles[1].setPos(Point(corner->x() + m_data->width/2.0, 
                    corner->y() + HANDLE_SIZE/2));

        m_data->resizeHandles[2].setId(HANDLE_RESIZE_NE);
        m_data->resizeHandles[2].setPos(Point(corner->x() + m_data->width -
                    HANDLE_SIZE/2, corner->y() + HANDLE_SIZE/2));

        m_data->resizeHandles[3].setId(HANDLE_RESIZE_W);
        m_data->resizeHandles[3].setPos(Point(corner->x() + HANDLE_SIZE/2, 
                    corner->y() + m_data->height/2.0 - HANDLE_SIZE/2));

        m_data->resizeHandles[4].setId(HANDLE_RESIZE_E);
        m_data->resizeHandles[4].setPos(Point(
                    corner->x() + m_data->width - HANDLE_SIZE/2,
                    corner->y() + m_data->height/2.0 - HANDLE_SIZE/2));

        m_data->resizeHandles[5].setId(HANDLE_RESIZE_SW);
        m_data->resizeHandles[5].setPos(Point(corner->x() + HANDLE_SIZE/2,
                    corner->y() + m_data->height - HANDLE_SIZE/2));

        m_data->resizeHandles[6].setId(HANDLE_RESIZE_S);
        m_data->resizeHandles[6].setPos(Point(
                    corner->x() + m_data->width/2.0 + HANDLE_SIZE/2,
                    corner->y() + m_data->height - HANDLE_SIZE/2));

        m_data->resizeHandles[7].setId(HANDLE_RESIZE_SE);
        m_data->resizeHandles[7].setPos(Point(
                    corner->x() + m_data->width - HANDLE_SIZE/2,
                    corner->y() + m_data->height - HANDLE_SIZE/2));

    }
    void Element::updateBoundingbox()
    {
    }

    std::vector<Handle *> Element::handles()
    {
        std::vector<Handle *> handles;
        for (int i = 0; i < 8; ++i)
            handles.push_back(&m_data->resizeHandles[i]);
        return handles;
    }
    Rectangle Element::boundingBox()
    {
        Point pos = Element::corner();
        double width = Element::width();
        double height = Element::height();

        Rectangle rect;
        rect.setX(pos.x());
        rect.setY(pos.y());
        rect.setWidth(width);
        rect.setHeight(height);

        return rect;
    }
    Rectangle Element::enclosingBox()
    {
        Point pos = Element::corner();
        double width = Element::width();
        double height = Element::height();

        Rectangle rect;
        rect.setX(pos.x());
        rect.setY(pos.y());
        rect.setWidth(width);
        rect.setHeight(height);

        return rect;
    }

    void Element::addUpdates(IDoc *doc)
    {
        if (NULL == doc)
            return;

        Rectangle rect = enclosingBox();
        rect.adjust(-5, -5, 5, 5);
        doc->addUpdate(&rect);

        updateHandles();

        typedef std::vector<Handle *> Vector;
        typedef std::vector<Handle *>::iterator It;

        Vector vector = Element::handles();
        It it;

        for (it = vector.begin(); it != vector.end(); it++) {
            Handle *h = *it;
            h->addUpdate(doc);
        }
    }

    GraphObjectChange *Element::moveHandle(HandleId id,
                                           Point *to,
                                           HandleMoveReason reason)
    {
        if (id < HANDLE_RESIZE_NW)
            return NULL;
        if (id > HANDLE_RESIZE_SE)
            return NULL;

        Point *corner = &m_data->corner;
        Point p = *to;

        point_sub(&p, &m_data->corner);

        switch(id) {
            case HANDLE_RESIZE_NW:
                if ( to->x() < (corner->x() + m_data->width) ) {
                    corner->setX( corner->x() + p.x() );
                    m_data->width -= p.x();
                }
                if ( to->y() < (corner->y() + m_data->height) ) {
                    corner->setY( corner->y() + p.y() );
                    m_data->height -= p.y();
                }
                break;
            case HANDLE_RESIZE_N:
                if ( to->y() < (corner->y() + m_data->height) ) {
                    corner->setY( corner->y() + p.y() );
                    m_data->height -= p.y();
                }
                break;
            case HANDLE_RESIZE_NE:
                if (p.x() > 0.0)
                    m_data->width = p.x();
                if ( to->y() < (corner->y() + m_data->height) ) {
                    corner->setY( corner->y() + p.y() );
                    m_data->height -= p.y();
                }
                break;
            case HANDLE_RESIZE_W:
                if ( to->x() < ( corner->x() + m_data->width) ) {
                    corner->setX( corner->x() + p.x() );
                    m_data->width -= p.x();
                }
                break;
            case HANDLE_RESIZE_E:
                if (p.x() > 0.0)
                    m_data->width = p.x();
                break;
            case HANDLE_RESIZE_SW:
                if ( to->x() < (corner->x() + m_data->width) ) {
                    corner->setX( corner->x() + p.x() );
                    m_data->width -= p.x();
                }
                if (p.y() > 0.0)
                    m_data->height = p.y();
                break;
            case HANDLE_RESIZE_S:
                if (p.y() > 0.0)
                    m_data->height = p.y();
                break;
            case HANDLE_RESIZE_SE:
                if (p.x() > 0.0)
                    m_data->width = p.x();
                if (p.y() > 0.0)
                    m_data->height = p.y();
                break;
            default:
                break;
        }
        return NULL;
    }


}


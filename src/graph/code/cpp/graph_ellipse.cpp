#include "graph_ellipse.h"

#include <QDomDocument>

#include <QDebug>
#include <math.h>

namespace GraphLib
{
    const static Ellipse::AspectType ASPECT_TYPE = Ellipse::FREE_ASPECT;
    const static bool SHOW_BACKGROUND = true;
    const static Color FOREGROUND_COLOR = Color(0, 0, 0);
    const static Color BACKGROUND_COLOR = Color(255, 255, 255);
    const static double LINE_WIDTH = 0.1;
    const static double DASH_LENGTH = 1.0;
    const static LineStyle LINE_STYLE = Qt::SolidLine;

    struct Ellipse::PrivateData
    {
        Handle centerHandle;

        double borderWidth;
        Color borderColor;
        Color innerColor;
        bool showBackground;
        Ellipse::AspectType aspectType;
        LineStyle lineStyle;
        double dashLength;
    };

    Ellipse::Ellipse() :
        Element(8)
    {
        m_data = new PrivateData;

        m_data->centerHandle.setId(HANDLE_CUSTOM1);
        m_data->centerHandle.setType(HANDLE_MAJOR_CONTROL);

        m_data->aspectType = ASPECT_TYPE;
        m_data->showBackground = SHOW_BACKGROUND;
        m_data->innerColor = BACKGROUND_COLOR;

        m_data->borderWidth = LINE_WIDTH;
        m_data->borderColor = FOREGROUND_COLOR;
        m_data->lineStyle = LINE_STYLE;
        m_data->dashLength = DASH_LENGTH;
    }

    Ellipse::~Ellipse()
    {
        delete m_data;
    }

    Point Ellipse::pos()
    {
        return corner();
    }

    void Ellipse::setPos(Point p)
    {
        setCorner(p);
    }

    void Ellipse::draw(GraphRenderer *renderer)
    {
        if (!renderer)
            return;

        Element *elem = this;
        Point center = elem->corner();

        if (m_data->showBackground) {
          renderer->setFillStyle(Qt::SolidPattern);
          renderer->fillEllipse(&center,
                                elem->width(),
                                elem->height(),
                                &m_data->innerColor);
        }

        renderer->setLineWidth(m_data->borderWidth);
        renderer->setLineStyle(m_data->lineStyle);
        renderer->setDashLength(m_data->dashLength);

        renderer->drawEllipse(&center,
                              elem->width(),
                              elem->height(),
                              &m_data->borderColor);
    }

    void Ellipse::setData(QString &xml)
    {
        QDomDocument doc("xml");
        if (!doc.setContent(xml, false))
            return;

        QDomElement root = doc.documentElement();
        if (root.tagName() != "data")
            return;

        QDomNodeList nodes = root.childNodes();
        for (int i = 0; i < nodes.count(); i++) {
            QDomNode node = nodes.at(i);

            QDomElement element = node.toElement();
            if (element.tagName() == "geometry") {
                double x = element.attribute("x").toDouble();
                double y = element.attribute("y").toDouble();
                double width = element.attribute("width").toDouble();
                double height = element.attribute("height").toDouble();

                setCorner(Point(x, y));
                setWidth(width);
                setHeight(height);
            }
        }
    }

    void Ellipse::setProperty(QString &xml)
    {
        QDomDocument doc("xml");
        if (!doc.setContent(xml, false))
            return;

        QDomElement root = doc.documentElement();
        if (root.tagName() != "property")
            return;

        QDomNode n = root.firstChild();
        while (!n.isNull()) {
            QDomElement e = n.toElement();
            QString tagName = e.tagName();

            if (tagName == "border-width")
                setBorderWidth(e.text().toDouble());
            if (tagName == "border-color")
                setBorderColor(Color(e.text()));
            if (tagName == "inner-color")
                setInnerColor(Color(e.text()));

            if (tagName == "show-background")
                setShowBackground(e.text().toInt());
            if (tagName == "aspect-type")
                setAspectType((Ellipse::AspectType)e.text().toInt());
            if (tagName == "line-style")
                setLineStyle((GraphLib::LineStyle)e.text().toInt());
            if (tagName == "dash-length")
                setDashLength(e.text().toDouble());

            n = n.nextSibling();
        }
    }

    void Ellipse::updateHandles()
    {
        Element::updateHandles();

        m_data->centerHandle.setPos(Point(corner().x() + width() / 2,
                                          corner().y() + height() / 2));
    }

    std::vector<Handle *> Ellipse::handles()
    {
        std::vector<Handle *> handles = Element::handles();
        handles.push_back(&m_data->centerHandle);
        return handles;
    }

    void Ellipse::selectf(Point *clickedPoint,
            GraphRenderer *interactiveRenderer)
    {
        updateHandles();
    }

    double Ellipse::distanceFrom(Point *point)
    {
        Element *elem = this;
        Point center;

        center.setX(elem->corner().x() + elem->width() / 2);
        center.setY(elem->corner().y() + elem->height() / 2);

        return distance_ellipse_point(&center,
                                      elem->width(),
                                      elem->height(),
                                      m_data->borderWidth,
                                      point);
    }

    void Ellipse::updateData()
    {
        Element *elem = this;

        if (m_data->aspectType == CIRCLE_ASPECT){
            float size = elem->height() < elem->width() ? elem->height() : elem->width();
            elem->setWidth(size);
            elem->setHeight(size);
        }

        updateHandles();
    }

    GraphObjectChange *Ellipse::move(Point *to)
    {
        setCorner(*to);
        updateData();
        return NULL;
    }

    GraphObjectChange *Ellipse::moveHandle(Handle *handle,
                                           Point *to,
                                           HandleMoveReason reason)
    {
        Element *elem = this;
        Point nw_to;
        Point se_to;

        if (handle == NULL)
            return NULL;
        if (to == NULL)
            return NULL;

        if (handle->id() >= 8 && handle->id() != HANDLE_CUSTOM1)
            return NULL;

        if (handle->id() == HANDLE_CUSTOM1) {
            Point delta;
            Point corner_to;
            delta.setX( to->x() - (elem->corner().x() + elem->width() / 2) );
            delta.setY( to->y() - (elem->corner().y() + elem->height() / 2) );
            corner_to.setX( elem->corner().x() + delta.x() );
            corner_to.setY( elem->corner().y() + delta.y() );
            return move(&corner_to);
        } else {
            if (m_data->aspectType != FREE_ASPECT){
                double width;
                double height;
                double new_width;
                double new_height;
                double to_width;
                double aspect_width;
                Point center;

                width = elem->width();
                height = elem->height();
                center.setX( elem->corner().x() + width / 2 );
                center.setY( elem->corner().y() + height / 2 );
                switch (handle->id()) {
                      case HANDLE_RESIZE_E:
                      case HANDLE_RESIZE_W:
                          new_width = 2 * fabs(to->x() - center.x());
                          new_height = new_width / width * height;
                          break;
                      case HANDLE_RESIZE_N:
                      case HANDLE_RESIZE_S:
                          new_height = 2 * fabs(to->y() - center.y());
                          new_width = new_height / height * width;
                          break;
                      case HANDLE_RESIZE_NW:
                      case HANDLE_RESIZE_NE:
                      case HANDLE_RESIZE_SW:
                      case HANDLE_RESIZE_SE:
                          to_width = 2 * fabs(to->x() - center.x());
                          aspect_width = 2 * fabs(to->y() - center.y()) / height * width;
                          new_width = to_width < aspect_width ? to_width : aspect_width;
                          new_height = new_width / width * height;
                          break;
                      default:
                          new_width = width;
                          new_height = height;
                          break;
                }

                nw_to.setX( center.x() - new_width / 2 );
                nw_to.setY( center.y() - new_height / 2 );
                se_to.setX( center.x() + new_width / 2 );
                se_to.setY( center.y() + new_height / 2 );

                elem->moveHandle(HANDLE_RESIZE_NW, &nw_to, reason);
                elem->moveHandle(HANDLE_RESIZE_SE, &se_to, reason);
            } else {
                Point center;
                Point opposite_to;
                center.setX( elem->corner().x() + elem->width() / 2 );
                center.setY( elem->corner().y() + elem->height() / 2 );
                opposite_to.setX( center.x() - (to->x() - center.x()) );
                opposite_to.setY( center.y() - (to->y() - center.y()) );

                elem->moveHandle(handle->id(), to, reason);
                elem->moveHandle(HandleId(7 - handle->id()), &opposite_to, reason);
            }

          updateData();
          return NULL;
        }
    }

    double Ellipse::borderWidth()
    {
        return m_data->borderWidth;
    }

    void Ellipse::setBorderWidth(double width)
    {
        m_data->borderWidth = width;
    }

    Color Ellipse::borderColor()
    {
        return m_data->borderColor;
    }

    void Ellipse::setBorderColor(Color color)
    {
        m_data->borderColor = color;
    }

    Color Ellipse::innerColor()
    {
        return m_data->innerColor;
    }

    void Ellipse::setInnerColor(Color color)
    {
        m_data->innerColor = color;
    }

    bool Ellipse::showBackground()
    {
        return m_data->showBackground;
    }

    void Ellipse::setShowBackground(bool show)
    {
        m_data->showBackground = show;
    }

    Ellipse::AspectType Ellipse::aspectType()
    {
        return m_data->aspectType;
    }

    void Ellipse::setAspectType(Ellipse::AspectType type)
    {
        m_data->aspectType = type;
    }

    LineStyle Ellipse::lineStyle()
    {
        return m_data->lineStyle;
    }

    void Ellipse::setLineStyle(LineStyle style)
    {
        m_data->lineStyle = style;
    }

    double Ellipse::dashLength()
    {
        return m_data->dashLength;
    }

    void Ellipse::setDashLength(double length)
    {
        m_data->dashLength = length;
    }

    GraphLib::Object *Ellipse::copy()
    {
        Ellipse *ellipse = new Ellipse();

        ellipse->setCorner(corner());
        ellipse->setWidth(width());
        ellipse->setHeight(height());

        ellipse->setBorderWidth(borderWidth());
        ellipse->setBorderColor(borderColor());
        ellipse->setInnerColor(innerColor());
        ellipse->setShowBackground(showBackground());
        ellipse->setAspectType(aspectType());
        ellipse->setLineStyle(lineStyle());
        ellipse->setDashLength(dashLength());

        return ellipse;
    }
}



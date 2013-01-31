#include "graph_box.h"
#include "graph_renderer.h"
#include "graph_element.h"
#include "igraph_doc.h"

#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDebug>

namespace GraphLib
{
    const static double BORDER_WIDTH = 0.5;
    const static Color  BORDER_COLOR = Color(0, 0, 0);
    const static Color  INNER_COLOR = Color(255, 255, 255);
    const static double BOX_WIDTH = 100.0;
    const static double BOX_HEIGHT = 100.0;

    const static LineStyle    BORDER_STYLE = Qt::SolidLine;

    struct Box::PrivateData
    {
        Point pos;

        float borderWidth;
        Color borderColor;
        Color innerColor;

        bool showBackground;
        LineStyle borderStyle;
        float dashLength;
        float cornerRadius;
    };

    Box::Box() :
        Element(8)
    {
        m_data = new PrivateData;
        m_data->borderColor = BORDER_COLOR;
        m_data->borderWidth = BORDER_WIDTH;
        m_data->innerColor = INNER_COLOR;
        m_data->borderStyle = BORDER_STYLE;
        m_data->showBackground = false;
        m_data->dashLength = 2.0;

        setWidth(BOX_WIDTH);
        setHeight(BOX_HEIGHT);
        //setCorner(pos);
    }

    Box::~Box()
    {
        delete m_data;
    }
    void Box::draw(GraphRenderer *renderer)
    {
        if (!renderer)
            return;

        Element *element = this;
        Point corner = element->corner();
        Point lrCorner;
        lrCorner.setX(corner.x() + element->width());
        lrCorner.setY(corner.y() + element->height());

        if (m_data->showBackground) {
            renderer->setFillStyle(Qt::SolidPattern);
            renderer->fillRect(&corner, &lrCorner, 
                    &m_data->innerColor);
        }

        renderer->setLineWidth(m_data->borderWidth);
        renderer->setLineStyle(m_data->borderStyle);
        if (m_data->borderStyle == Qt::DashLine)
            renderer->setDashLength(m_data->dashLength);
        renderer->setLineJoin(Qt::RoundJoin);

        renderer->drawRect(&corner, &lrCorner,
                &m_data->borderColor);
    }

    Color Box::fillColor()
    {
        return m_data->innerColor;
    }
    void Box::setFillColor(Color color)
    {
        m_data->innerColor = color;
    }

    double Box::borderWidth()
    {
        return m_data->borderWidth;
    }
    void Box::setBorderWidth(double width)
    {
        m_data->borderWidth = width;
    }

    void Box::selectf(Point *clickedPoint, 
            GraphRenderer *interactiveRenderer)
    {
        Element::updateHandles();
    }

    double Box::distanceFrom(Point *point)
    {
        if (!point)
            return 0.0;

        Element *obj = this;
        Point leftTop = obj->corner();
        Rectangle rect(leftTop.x(), leftTop.y(), obj->width(), obj->height());

        return distance_rectangle_point(&rect, point);
    }

    bool Box::backgroundVisible() const
    {
        return m_data->showBackground;
    }
    void Box::setBackgroundVisible(bool visible)
    {
        m_data->showBackground = visible;
    }

    void Box::setBorderStyle(LineStyle mode)
    {
        m_data->borderStyle = mode;
    }
    LineStyle Box::borderStyle()
    {
        return m_data->borderStyle;
    }

    void Box::setBorderColor(Color color)
    {
        m_data->borderColor = color;
    }
    Color Box::borderColor()
    {
        return m_data->borderColor;
    }


    void readGeometry(Box *b, QXmlStreamReader &xml) 
    {
        Q_ASSERT(xml.isStartElement() && xml.name() == "geometry");

        double x = xml.attributes().value("x").toString().toDouble();
        double y = xml.attributes().value("y").toString().toDouble();
        double width = xml.attributes().value("width").toString().toDouble();
        double height = xml.attributes().value("height").toString().toDouble();

        Element *element = b;
        element->setCorner(QPointF(x, y));
        element->setWidth(width);
        element->setHeight(height);
    }

    void readData(Box *b, QXmlStreamReader &xml)
    {
        Q_ASSERT(xml.isStartElement() && xml.name() == "data");

        while (xml.readNextStartElement()) {
            if (xml.name() == "geometry") {
                readGeometry(b, xml);
            }
        }
    }

    GraphLib::Object *Box::copy()
    {
        Box *obj = new Box();

        obj->setCorner(corner());
        obj->setWidth(width());
        obj->setHeight(height());

        obj->setBorderWidth(borderWidth());
        obj->setBorderColor(borderColor());
        obj->setFillColor(fillColor());

        obj->setBackgroundVisible(backgroundVisible());
        obj->setBorderStyle(borderStyle());

        return obj;
    }

    void Box::setData(QString &data)
    {
        QXmlStreamReader xml(data);

        if (xml.readNextStartElement()) {
            if (xml.name() == "data") {
                readData(this, xml);
            }
        }
    }
    void Box::setProperty(QString &xml)
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
                m_data->borderWidth = e.text().toDouble();
            if (tagName == "border-color")
                m_data->borderColor = Color(e.text());
            if (tagName == "inner-color")
                m_data->innerColor = Color(e.text());

            if (tagName == "show-background")
                m_data->showBackground = e.text().toInt();
            if (tagName == "border-style")
                m_data->borderStyle = (GraphLib::LineStyle)e.text().toInt();
            if (tagName == "dash-length")
                m_data->dashLength = e.text().toDouble();

            n = n.nextSibling();
        }
    }

    void Box::updateHandles()
    {
        Element::updateHandles();
    }

    std::vector<Handle *> Box::handles()
    {
        std::vector<Handle *> handles = Element::handles();
        return handles;
    }

    GraphObjectChange *Box::moveHandle(Handle *handle,
                                  Point *to,
                                  HandleMoveReason reason)
    {
        Element *elem = this;
        elem->moveHandle(handle->id(), to, reason);
        updateHandles();
        return NULL;
    }

    Point Box::pos()
    {
        return m_data->pos;
    }
    void Box::setPos(Point pos)
    {
        m_data->pos = pos;
        Element::setCorner(pos);
    }
    void Box::setCorner(Point pos)
    {
        m_data->pos = pos;
        Element::setCorner(pos);
    }
}



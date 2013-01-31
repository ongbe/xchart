#include "graph_polyline.h"
#include "graph_polyconn.h"
#include "graph_renderer.h"
#include "graph_element.h"
#include "igraph_doc.h"

#include "graph_geometry.h"
#include "graph_bounding_box.h"

#include <QVector>
#include <QDomDocument>
#include <QDebug>

namespace GraphLib
{
    const static double BORDER_WIDTH = 0.5;
    const static Color  BORDER_COLOR = Color(0, 0, 0);
    const static Color  INNER_COLOR = Color(255, 255, 255);
    const static double BOX_WIDTH = 100.0;
    const static double BOX_HEIGHT = 100.0;

    const static LineStyle    BORDER_STYLE = Qt::SolidLine;

    struct Polyline::PrivateData
    {
        float borderWidth;
        Color borderColor;
        Color innerColor;

        bool showBackground;
        LineStyle borderStyle;
        float dashLength;
        float cornerRadius;
        IDoc *doc;
        Polyconn conn;
        Point clickedPoint;
    };

    Polyline::Polyline() :
        Element(3)
    {
        m_data = new PrivateData;
        m_data->borderColor = BORDER_COLOR;
        m_data->borderWidth = BORDER_WIDTH;
        m_data->innerColor = INNER_COLOR;
        m_data->borderStyle = BORDER_STYLE;
        m_data->showBackground = false;
        m_data->dashLength = 2.0;
        m_data->clickedPoint = Point(0, 0);

        setWidth(BOX_WIDTH);
        setHeight(BOX_HEIGHT);
        m_data->doc = NULL;
    }

    Polyline::~Polyline()
    {
        delete m_data;
    }
    void Polyline::draw(GraphRenderer *renderer)
    {
        if (!renderer)
            return;

        int num = m_data->conn.points().size();
        /*
        Point corner = m_data->conn.corner();
        QTransform transform;
        transform.translate(corner.x(), corner.y());
        QVector<Point> points;
        for (int i = 0; i < num; ++i) {
            Point p = transform.map(m_data->conn.points().at(i));
            points << p;
        }
        */

        QVector<Point> points = m_data->conn.points();
        renderer->setLineWidth(m_data->borderWidth);
        renderer->drawPolyline(points.data(), num, 
                &m_data->borderColor);
    }

    Color Polyline::fillColor()
    {
        return m_data->innerColor;
    }
    void Polyline::setFillColor(Color color)
    {
        m_data->innerColor = color;
    }

    double Polyline::borderWidth()
    {
        return m_data->borderWidth;
    }
    void Polyline::setBorderWidth(double width)
    {
        m_data->borderWidth = width;
    }

    void Polyline::selectf(Point *clickedPoint, 
            GraphRenderer *interactiveRenderer)
    {
        updateHandles();
        m_data->conn.updateHandles();
    }

    double Polyline::distanceFrom(Point *point)
    {
        if (!point)
            return 0.0;
        QVector<Point> points = m_data->conn.points();
        Point *poly = &points[0];
        int npoints = points.size();
        double lineWidth = m_data->borderWidth;
        return distance_polygon_point(poly, npoints,
                lineWidth, point);
    }

    bool Polyline::backgroundVisible() const
    {
        return m_data->showBackground;
    }
    void Polyline::setBackgroundVisible(bool visible)
    {
        m_data->showBackground = visible;
    }

    void Polyline::setBorderStyle(LineStyle mode)
    {
        m_data->borderStyle = mode;
    }
    LineStyle Polyline::borderStyle()
    {
        return m_data->borderStyle;
    }

    void Polyline::setBorderColor(Color color)
    {
        m_data->borderColor = color;
    }
    Color Polyline::borderColor()
    {
        return m_data->borderColor;
    }

    Rectangle Polyline::boundingBox()
    {
        return m_data->conn.boundingBox();
    }

    Rectangle Polyline::enclosingBox()
    {
        return m_data->conn.boundingBox();
    }

    void Polyline::addUpdates(IDoc *doc)
    {
        if (doc) {
            Rectangle rect = enclosingBox();
            doc->addUpdate(&rect);
        }

        typedef std::vector<Handle *> Vector;
        typedef std::vector<Handle *>::iterator It;

        Vector vector = m_data->conn.handles();
        It it;

        for (it = vector.begin(); it != vector.end(); it++) {
            Handle *h = *it;
            h->addUpdate(doc);
        }
    }

    GraphObjectChange *Polyline::move(Point *to)
    {
        setCorner(*to);
        m_data->conn.move(&m_data->clickedPoint, to);
        updateHandles();
    }

    GraphObjectChange *Polyline::moveHandle(Handle *h, 
            Point *to, HandleMoveReason reason)
    {
        m_data->conn.moveHandle(h, to, reason);
        updateHandles();
        m_data->conn.updateHandles();
    }

    IDoc *Polyline::doc()
    {
        return m_data->doc;
    }
    void Polyline::setDoc(IDoc *doc)
    {
        m_data->doc = doc;
    }
    void Polyline::updateHandles()
    {
        PolyBBExtras *extra = m_data->conn.extraSpacing();

        extra->startTrans = m_data->borderWidth / 2.0;
        extra->endTrans = m_data->borderWidth / 2.0;
        extra->middleTrans = m_data->borderWidth / 2.0;
        extra->startLong = m_data->borderWidth / 2.0;
        extra->endLong = m_data->borderWidth / 2.0;
        m_data->conn.updateHandles();
    }

    std::vector<Handle *> Polyline::handles()
    {
        return m_data->conn.handles();
    }

    void Polyline::addPoint(const Point &point)
    {
        m_data->conn.addPoint(point);
    }
    void Polyline::removePoint(const Point &point)
    {
        m_data->conn.removePoint(point);
    }

    void Polyline::setData(QString &xmlData)
    {
        QDomDocument doc("xml");
        if (!doc.setContent(xmlData, false))
            return;

        QDomElement root = doc.documentElement();
        if (root.tagName() != "data")
            return;

        m_data->conn.clearPoints();
        QDomNodeList nodes = root.childNodes();
        for (int i = 0; i < nodes.count(); i++) {
            QDomNode node = nodes.at(i);

            QDomElement element = node.toElement();
            if (element.tagName() == "geometry") {
                double x = element.attribute("x").toDouble();
                double y = element.attribute("y").toDouble();
                m_data->conn.addPoint(Point(x,y));
            }
        }
    }

    void Polyline::setProperty(QString &xmlData)
    {
        QDomDocument doc("xml");
        if (!doc.setContent(xmlData, false))
            return;

        QDomElement root = doc.documentElement();
        if (root.tagName() != "property")
            return;

        QDomNodeList nodes = root.childNodes();
        for (int i = 0; i < nodes.count(); i++) {
            QDomNode node = nodes.at(i);
            QDomElement element = node.toElement();
            if (element.tagName() == "border") {
                double border = element.text().toDouble();
                setBorderWidth(border);
            }
            if (element.tagName() == "color") {
                QString color = element.text();
                QStringList list = color.split(",");
                if (list.size() == 3) {
                    int r = list.at(0).toInt();
                    int g = list.at(1).toInt();
                    int b = list.at(2).toInt();
                    Color color(r,g,b);
                    setBorderColor(color);
                }
            }
        }
    }

    GraphLib::Polyconn *Polyline::polyConn() 
    {
        return &m_data->conn;
    }
    Object *Polyline::copy()
    {
        Polyline *line = new Polyline;
        line->polyConn()->clearPoints();
        line->polyConn()->setOPoints(m_data->conn.OPoints());
        line->polyConn()->setCorner(m_data->conn.corner());
        return line;
    }
    Point Polyline::corner()
    {
        return m_data->conn.corner();
    }
    void Polyline::setCorner(Point p)
    {
        m_data->conn.setCorner(p);
    }
}



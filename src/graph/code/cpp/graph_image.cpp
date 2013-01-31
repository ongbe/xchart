#include "graph_image.h"
#include "graph_renderer.h"
#include "graph_element.h"
#include "igraph_doc.h"


#include <math.h>
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

    struct Image::PrivateData
    {
        float borderWidth;
        Color borderColor;
        Color innerColor;

        bool showBackground;
        LineStyle borderStyle;
        float dashLength;
        float cornerRadius;
        GraphImage image;
    };

    Image::Image() :
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
    }

    Image::~Image()
    {
        delete m_data;
    }
    void Image::draw(GraphRenderer *renderer)
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
        double width = Element::width();
        double height = Element::height();
        renderer->drawImage(&corner, width, height,
                &m_data->image);
    }

    Color Image::fillColor()
    {
        return m_data->innerColor;
    }
    void Image::setFillColor(Color color)
    {
        m_data->innerColor = color;
    }

    double Image::borderWidth()
    {
        return m_data->borderWidth;
    }
    void Image::setBorderWidth(double width)
    {
        m_data->borderWidth = width;
    }

    void Image::selectf(Point *clickedPoint, 
            GraphRenderer *interactiveRenderer)
    {
        Element::updateHandles();
    }

    double Image::distanceFrom(Point *point)
    {
        if (!point)
            return 0.0;

        Element *obj = this;
        Point leftTop = obj->corner();
        Rectangle rect(leftTop.x(), leftTop.y(), obj->width(), obj->height());

        return distance_rectangle_point(&rect, point);
    }

    bool Image::backgroundVisible() const
    {
        return m_data->showBackground;
    }
    void Image::setBackgroundVisible(bool visible)
    {
        m_data->showBackground = visible;
    }

    void Image::setBorderStyle(LineStyle mode)
    {
        m_data->borderStyle = mode;
    }
    LineStyle Image::borderStyle()
    {
        return m_data->borderStyle;
    }

    void Image::setBorderColor(Color color)
    {
        m_data->borderColor = color;
    }
    Color Image::borderColor()
    {
        return m_data->borderColor;
    }

    void Image::setImage(GraphImage &image)
    {
        m_data->image = image;
    }

    Rectangle Image::boundingBox()
    {
        Point pos = Element::corner();
        double width = Element::width();
        double height = Element::height();

        Rectangle rect;
        rect.setX(pos.x() - 2.0);
        rect.setY(pos.y() - 2.0);
        rect.setWidth(width + 4.0);
        rect.setHeight(height + 4.0);

        return rect;
    }
    Rectangle Image::enclosingBox()
    {
        Point pos = Element::corner();
        double width = Element::width();
        double height = Element::height();

        Rectangle rect;
        rect.setX(pos.x() - 5.0);
        rect.setY(pos.y() - 5.0);
        rect.setWidth(width + 10.0);
        rect.setHeight(height + 10.0);

        return rect;
    }

    void Image::addUpdates(IDoc *doc)
    {
        if (doc) {
            Rectangle rect = enclosingBox();
            doc->addUpdate(&rect);
        }

        typedef std::vector<Handle *> Vector;
        typedef std::vector<Handle *>::iterator It;

        Vector vector = Element::handles();
        It it;

        for (it = vector.begin(); it != vector.end(); it++) {
            Handle *h = *it;
            h->addUpdate(doc);
        }
    }
    std::vector<Handle *> Image::handles()
    {
        std::vector<Handle *> handles = Element::handles();
        return handles;
    }

    GraphObjectChange *Image::moveHandle(Handle *handle,
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

        double width;
        double height;

        Point old_corner = elem->corner();
        Point new_corner = old_corner;

        width = elem->width();
        height = elem->height();
        double new_width = width;
        double new_height = height;
        switch (handle->id()) {
            case HANDLE_RESIZE_E:
                new_width = to->x() - old_corner.x();
                new_height = height;
                new_corner = old_corner;
                break;
            case HANDLE_RESIZE_W:
                new_width = width + (old_corner.x() - to->x());
                new_height = height;
                new_corner = Point(to->x(), old_corner.y());
                break;
            case HANDLE_RESIZE_N:
                new_width = width;
                new_height = height + (old_corner.y() - to->y());
                new_corner = Point(old_corner.x(), to->y());
                break;
            case HANDLE_RESIZE_S:
                new_width = width;
                new_height = to->y() - old_corner.y();
                new_corner = old_corner;
                break;
            case HANDLE_RESIZE_NW:
                new_width = width + old_corner.x() - to->x();
                new_height = height + old_corner.y() - to->y();
                new_corner = Point(to->x(), to->y());
                break;
            case HANDLE_RESIZE_NE:
                new_width = to->x() - old_corner.x();
                new_height = height + old_corner.y() - to->y();
                new_corner = Point(old_corner.x(), to->y());
                break;
            case HANDLE_RESIZE_SW:
                new_width = width + old_corner.x() - to->x();
                new_height = to->y() - old_corner.y();
                new_corner = Point(to->x(), old_corner.y());
                break;
            case HANDLE_RESIZE_SE:
                new_width = to->x() - old_corner.x();
                new_height = to->y() - old_corner.y();
                new_corner = old_corner;
                break;
            default:
                new_width = width;
                new_height = height;
                break;
        }

        setCorner(new_corner);
        setWidth(new_width);
        setHeight(new_height);
        updateHandles();
        return NULL;
    }

    void Image::setData(QString &xmlData)
    {
        QDomDocument doc("xml");
        if (!doc.setContent(xmlData, false))
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
                setCorner(Point(x,y));
                setWidth(width);
                setWidth(height);
            }
            if (element.tagName() == "image") {
                QString path = element.text();
                m_data->image = QImage(path);
            }
        }
    }
    void Image::setProperty(QString &xmlData)
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
    Object *Image::copy()
    {
        Image *image = new Image;
        image->setBorderColor(m_data->borderColor);
        image->setBorderWidth(m_data->borderWidth);
        image->setCorner(corner());
        image->setImage(m_data->image);
        return image;
    }

    void Image::setPos(Point p)
    {
        setCorner(p);
    }
    Point Image::pos()
    {
        return corner();
    }
}



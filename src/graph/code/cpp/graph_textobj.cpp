#include "graph_textobj.h"
#include "graph_text_line.h"

#include <QDomDocument>

#include <QDebug>

namespace GraphLib
{
    struct Textobj::PrivateData
    {
        QString text;
        std::list<GraphTextLine *> lines;

        Handle textHandle;

        bool showBackground;
        Color fillColor;

        GraphLib::GraphFont *font;
        Color fontColor;
        Qt::Alignment alignment;
    };

    Textobj::Textobj() :
        Element(8)
    {
        m_data = new PrivateData;

        m_data->textHandle.setId(HANDLE_CUSTOM1);
        m_data->textHandle.setType(HANDLE_MAJOR_CONTROL);

        m_data->font = new GraphLib::GraphFont();

        m_data->fillColor = Color(255, 255, 255);

        m_data->fontColor = Color(0, 0, 0);
        m_data->alignment = Qt::AlignCenter;
        m_data->text = "test";
        m_data->showBackground = true;
    }

    Textobj::~Textobj()
    {
        delete m_data;
    }

    QString Textobj::text()
    {
        return m_data->text;
    }

    void Textobj::setText(QString text)
    {
        m_data->text = text;
        setString(&text);
    }

    GraphLib::GraphFont * Textobj::font()
    {
        return m_data->font;
    }

    GraphLib::Color Textobj::fontColor()
    {
        return m_data->fontColor;
    }

    void Textobj::setFontColor(GraphLib::Color color)
    {
        m_data->fontColor = color;
    }


    GraphLib::Color Textobj::fillColor()
    {
        return m_data->fillColor;
    }

    void Textobj::setFillColor(GraphLib::Color color)
    {
        m_data->fillColor = color;
    }

    GraphLib::Alignment Textobj::alignment()
    {
        return m_data->alignment;
    }

    void Textobj::setAlignment(GraphLib::Alignment alignment)
    {
        m_data->alignment = alignment;
    }

    bool Textobj::showBackground()
    {
        return m_data->showBackground;
    }

    void Textobj::setShowBackground(bool show)
    {
        m_data->showBackground = show;
    }

    void Textobj::setFont(GraphLib::GraphFont *font)
    {
        if (font == NULL)
            return;

        if (m_data->font != NULL)
            delete m_data->font;

        m_data->font = font;
    }

    void Textobj::draw(GraphRenderer *renderer)
    {
        if (renderer == NULL)
            return;

        Point point = corner();

        if (m_data->showBackground) {
            Point topLeft = boundingBox().topLeft();
            Point bottomRight = boundingBox().bottomRight();
            renderer->fillRect(&topLeft, &bottomRight, &m_data->fillColor);
        }

        Point pos = point;

        typedef std::list<GraphTextLine *> ListType;
        typedef std::list<GraphTextLine *>::iterator ItType;
        ListType &lines = m_data->lines;
        ItType it;
        for (it = lines.begin(); it != lines.end(); ++it) {
            GraphTextLine *line = *it;

            GraphLib::GraphFont *font = line->font();

            bool bold = font->bold();
            bool italic = font->italic();
            int pixelSize = font->pixelSize();

            font->setBold(m_data->font->bold());
            font->setItalic(m_data->font->italic());
            font->setPixelSize(m_data->font->pixelSize());

            //renderer->setFont(m_data->font);
            renderer->drawTextLine(line, &pos, m_data->alignment, &m_data->fontColor);
            pos.setY(pos.y() + m_data->font->pixelSize());

            font->setBold(bold);
            font->setItalic(italic);
            font->setPixelSize(pixelSize);
        }

    }

    void Textobj::selectf(Point *clickedPoint,
                          GraphRenderer *interactiveRenderer)
    {
        updateHandles();
    }

    double Textobj::distanceFrom(Point *point)
    {
        if (!point)
            return 0.0;
        Rectangle rect = boundingBox();
        return distance_rectangle_point(&rect, point);
    }

    void Textobj::updateHandles()
    {
        m_data->textHandle.setPos(corner());
    }

    std::vector<Handle *> Textobj::handles()
    {
        std::vector<Handle *> handles;
        handles.push_back(&m_data->textHandle);
        return handles;
    }

    GraphObjectChange * Textobj::move(Point *to)
    {
        setCorner(*to);
        updateHandles();
        return NULL;
    }

    GraphObjectChange *Textobj::moveHandle(Handle *handle,
                                  Point *to,
                                  HandleMoveReason reason)
    {
        Element *elem = this;

        if (handle == NULL)
            return NULL;
        if (to == NULL)
            return NULL;

        if (handle->id() != HANDLE_CUSTOM1)
            return NULL;

        if (handle->id() == HANDLE_CUSTOM1) {
            Point delta;
            Point corner_to;
            delta.setX( to->x() - (elem->corner().x() + elem->width() / 2) );
            delta.setY( to->y() - (elem->corner().y() + elem->height() / 2) );
            corner_to.setX( elem->corner().x() + delta.x() );
            corner_to.setY( elem->corner().y() + delta.y() );
            return move(&corner_to);
        }

        return NULL;
    }

    void Textobj::setString(const QString *str)
    {
        if (str == NULL)
            return;
        QStringList list = str->split("\n", QString::SkipEmptyParts);
        std::list<GraphTextLine *> &lines = m_data->lines;
        GraphFont *font = m_data->font;
        float height = m_data->font->pixelSize();
        foreach(QString str, list) {
            GraphTextLine *line = new GraphTextLine(&str, font, height);
            lines.push_back(line);
        }
    }

    Rectangle Textobj::boundingBox()
    {
        Point pos = corner();
        float fontHeight = m_data->font->pixelSize();

        double left = pos.x();
        double top = pos.y();

        double width = 0;
        double height = 0;

        typedef std::list<GraphTextLine *> ListType;
        typedef std::list<GraphTextLine *>::iterator ItType;
        ListType &lines = m_data->lines;
        ItType it;
        for (it = lines.begin(); it != lines.end(); ++it) {
            GraphTextLine *line = *it;

            QString string = *line->string();
            double stringWidth = m_data->font->stringWidth(string);
            if (width <= stringWidth)
                width = stringWidth;

            height += fontHeight;
        }

        return Rectangle(left, top - fontHeight, width, height);
    }

    Rectangle Textobj::enclosingBox()
    {
        Rectangle rect = boundingBox();
        rect.setLeft( rect.left() - 5 );
        rect.setTop( rect.top() - 5 );
        rect.setWidth( rect.width() + 10 );
        rect.setHeight( rect.height() + 10 );
        return rect;
    }

    void Textobj::setData(QString &xml)
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

                QString text = element.attribute("text");

                Point pos(x, y);
                move(&pos);

                setText(text);
            }
        }
    }

    void Textobj::setProperty(QString &xml)
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

            if (tagName == "font-color")
                setFontColor(Color(e.text()));
            if (tagName == "fill-color")
                setFillColor(Color(e.text()));
            if (tagName == "alignment")
                setAlignment((GraphLib::Alignment)e.text().toInt());
            if (tagName == "show-background")
                setShowBackground(e.text().toInt());

            if (tagName == "font-bold")
                m_data->font->setBold(e.text().toInt());
            if (tagName == "font-italic")
                m_data->font->setItalic(e.text().toInt());
            if (tagName == "font-pixelsize")
                m_data->font->setPixelSize(e.text().toInt());

            n = n.nextSibling();
        }
    }


    GraphLib::Object *Textobj::copy()
    {
        Textobj *obj = new Textobj();

        obj->setCorner(corner());

        obj->setText(m_data->text);

        obj->font()->setBold(m_data->font->bold());
        obj->font()->setItalic(m_data->font->italic());
        obj->font()->setPixelSize(m_data->font->pixelSize());

        obj->setShowBackground(showBackground());
        obj->setFillColor(fillColor());
        obj->setFontColor(fontColor());
        obj->setAlignment(m_data->alignment);

        return obj;
    }
}



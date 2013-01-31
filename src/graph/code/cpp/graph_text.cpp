#include "graph_text.h"
#include "graph_color.h"
#include "graph_geometry.h"
#include "graph_object.h"
#include "graph_text_line.h"
#include "graph_renderer.h"

namespace GraphLib
{
    struct GraphText::PrivateData
    {
        /** The object that owns this text. */
        Object *parentObject;

        /* Text data: */
        int numLines;
        std::list<GraphTextLine *> lines;

        /* Attributes: */
        GraphFont *font;
        float height;
        Point pos;
        Color color;
        Alignment align;
    };

    GraphText::GraphText(const QString *string, GraphFont *font,
            float height, Point *pos, Color *color, 
            Alignment align)
    {
        m_data = new PrivateData;
        m_data->height = height;
        m_data->font = font;
        m_data->pos = *pos;
        m_data->color = *color;
        m_data->align = align;
        setString(string);
    }

    GraphText::~GraphText()
    {
        typedef std::list<GraphTextLine *> ListType;
        typedef std::list<GraphTextLine *>::iterator ItType;
        ListType &lines = m_data->lines;
        ItType it;
        for (it = lines.begin(); it != lines.end(); ++it) {
            delete (*it);
        }

        delete m_data;
    }
    void GraphText::setString(const QString *str)
    {
        if (str == NULL)
            return;
        QStringList list = str->split("\n", QString::SkipEmptyParts);
        std::list<GraphTextLine *> &lines = m_data->lines;
        GraphFont *font = m_data->font;
        float height = m_data->height;
        foreach(QString str, list) {
            GraphTextLine *line = new GraphTextLine(&str, font, height);
            lines.push_back(line);
        }
    }

    void GraphText::draw(GraphRenderer *renderer)
    {
        if (renderer == NULL)
            return;

        Point pos = m_data->pos;
        Color color = m_data->color;
        Alignment align = m_data->align;
        float fontHeight = m_data->height;
        renderer->setFont(m_data->font);

        typedef std::list<GraphTextLine *> ListType;
        typedef std::list<GraphTextLine *>::iterator ItType;
        ListType &lines = m_data->lines;
        ItType it;
        for (it = lines.begin(); it != lines.end(); ++it) {
            GraphTextLine *line = *it;
            renderer->drawTextLine(line, &pos, align, &color);
            pos.setY(pos.y() + fontHeight);
        }
    }
}


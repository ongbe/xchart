#include "graph_text_line.h"
#include "graph_font.h"


#include <QString>
#include <QFont>
#include <QDebug>

namespace GraphLib
{
    struct GraphTextLine::PrivateData
    {
        /* 文本数据 */
        QString chars;

        /* 文本属性*/ 
        GraphFont *font;
        float height;
    };

    GraphTextLine::GraphTextLine(QString *text, GraphFont *font, float fontHeight)
    {
        m_data = new PrivateData;
        m_data->chars = (text == 0 ?QString(""):QString(*text));
        m_data->font = font;
        m_data->height = fontHeight;
    }

    GraphTextLine::~GraphTextLine()
    {
        delete m_data;
    }

    GraphTextLine *GraphTextLine::copy()
    {
        return NULL;
    }

    void GraphTextLine::setString(QString *string)
    {
        m_data->chars = *string;
    }
    QString *GraphTextLine::string()
    {
        return &m_data->chars;
    }
    void GraphTextLine::setFont(GraphFont *font)
    {
        m_data->font = font;
    }
    GraphFont *GraphTextLine::font()
    {
        return m_data->font;
    }
    void GraphTextLine::setHeight(float height)
    {
        m_data->height = height;
    }
    float GraphTextLine::height()
    {
        return m_data->height;
    }
    float GraphTextLine::width()
    {
        return 10.0;
    }
}



#include "graph_font.h"

#include <QFontMetrics>

namespace GraphLib
{
    struct GraphFont::PrivateData
    {
    };

    GraphFont::GraphFont()
    {
        m_data = new PrivateData;
    }
    GraphFont::~GraphFont()
    {
        delete m_data;
    }

    float GraphFont::height()
    {
        QFont *font = this;
        QFontMetrics fm(*font);
        return fm.height();
    }

    float GraphFont::stringWidth(QString &str)
    {
        QFont *font = this;
        QFontMetrics fm(*font);
        return fm.width(str);
    }
}



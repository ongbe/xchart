
#include "chart_label.h"

#include "graph_font.h"

using namespace Cross;

struct ChartLabel::PrivateData
{
    QString content;
    QColor outlineColor;
    QColor backgroundColor;
    QColor fontColor;
};

ChartLabel::ChartLabel(QString content)
{
    init();

    m_data->content = content;
}

ChartLabel::~ChartLabel()
{
    delete m_data;
}

void ChartLabel::init()
{
    m_data = new PrivateData;
    m_data->content = "text";
    m_data->backgroundColor = QColor(255, 255, 192);
    m_data->outlineColor = Qt::gray;
    m_data->fontColor = Qt::black;
}

QSizeF ChartLabel::size(GraphLib::GraphRenderer *renderer, QString content)
{
    GraphLib::GraphFont *font = renderer->font();
    double fontHeight = font->height();
    double fontWidth = font->stringWidth(content);

    return QSizeF(5 + fontWidth + 5,
                 3 + fontHeight + 5);
}

void ChartLabel::draw(GraphLib::GraphRenderer *renderer,
                      QPointF point)
{
    QString content = m_data->content;

    GraphLib::GraphFont *font = renderer->font();
    double fontHeight = font->height();
    double fontWidth = font->stringWidth(content);
    double left = point.x();
    double top = point.y() - fontHeight - 3 - 5;
    double right = point.x() + fontWidth + 5 + 5;
    double bottom = point.y();

    QRectF areaRect(left, top, right - left, bottom - top);

    drawBackground(renderer, areaRect);
    drawOutline(renderer, areaRect);
    drawContent(renderer, QPointF(left + 5, bottom - 5), content);
}

void ChartLabel::drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect)
{
    GraphLib::Point pointTopLeft = rect.topLeft();
    GraphLib::Point pointBottomRight = rect.bottomRight();
    GraphLib::Color color = m_data->backgroundColor;

    renderer->fillRect(&pointTopLeft, &pointBottomRight, &color);
}

void ChartLabel::drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect)
{
    GraphLib::Point pointTopLeft = rect.topLeft();
    GraphLib::Point pointBottomRight = rect.bottomRight();
    GraphLib::Color color = m_data->outlineColor;

    renderer->drawRect(&pointTopLeft, &pointBottomRight, &color);
}

void ChartLabel::drawContent(GraphLib::GraphRenderer *renderer,
                             QPointF point,
                             QString content)
{
    GraphLib::Color color = m_data->fontColor;

    renderer->drawString(content, &point, 0, &color);
}

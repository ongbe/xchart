
#include "chart_text_title.h"
#include "graph_font.h"
#include "graph_text.h"

#include <QFont>

namespace Cross
{
    struct ChartTextTitle :: PrivateData
    {
        bool visible;
        QString text;
        QPointF pos;

        QFont font;
        int textAlignment;
    };

    ChartTextTitle::ChartTextTitle()
    {
        m_data = new PrivateData;
        m_data->visible = true;
        m_data->pos.setX(0.0);
        m_data->pos.setY(0.0);
        m_data->textAlignment = 0;
    }

    ChartTextTitle::~ChartTextTitle()
    {
        delete m_data;
    }

    void ChartTextTitle::setText(const QString &text)
    {
        m_data->text = text;
    }

    QString ChartTextTitle::text() const
    {
        return m_data->text;
    }

    void ChartTextTitle::setTextAlignment(int alignment)
    {
        m_data->textAlignment = alignment;
    }

    int ChartTextTitle::textAlignment()
    {
        return m_data->textAlignment;
    }

    void ChartTextTitle::setPosition(const QPointF &pos)
    {
        m_data->pos = pos;
    }

    QPointF ChartTextTitle::getPosition() const
    {
        return m_data->pos;
    }

    void ChartTextTitle::setVisible(bool visible)
    {
        m_data->visible = visible;
    }

    bool ChartTextTitle::isVisible()
    {
        return m_data->visible;
    }

    void ChartTextTitle::setFont(const QFont &font)
    {
        m_data->font = font;
    }

    QFont ChartTextTitle::getFont() const
    {
        return m_data->font;
    }

    void ChartTextTitle::draw(GraphLib::GraphRenderer *renderer,
            QRectF titleArea)
    {
        if (renderer == NULL || m_data->text.isEmpty())
            return;
        GraphLib::Color color;

        // 绘制外边框
        GraphLib::Rectangle bbox = titleArea;
        GraphLib::Point ulCorner = bbox.topLeft();
        GraphLib::Point lrCorner = bbox.bottomRight();
        color = QColor(Qt::gray);
        renderer->drawRect(&ulCorner, &lrCorner, &color);
        // 绘制背景色

        //绘制内容
        QPointF point;
        color = Qt::black;
        QString content = m_data->text;

        float textHeight = renderer->font()->height();
        float textWidth = renderer->font()->stringWidth(content);
        bbox = QRectF(0, 0, textWidth, textHeight);
        bbox.moveCenter(titleArea.center());
        point = bbox.bottomLeft();
        renderer->drawString(content, &point, 0, &color);
    }


    void ChartTextTitle::drawVertical(GraphLib::GraphRenderer *renderer, QRectF titleArea)
    {
    }

    void ChartTextTitle::drawHorizontal(GraphLib::GraphRenderer *renderer, QRectF titleArea)
    {
    }

}

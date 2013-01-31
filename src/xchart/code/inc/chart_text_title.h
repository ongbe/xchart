
#ifndef _CHART_TEXT_TITLE_H_
#define _CHART_TEXT_TITLE_H_

#include "chart_title.h"
#include "graph_renderer.h"

namespace Cross
{
    class ChartTextTitle : public ChartTitle
    {
    public:
        ChartTextTitle();
        virtual ~ChartTextTitle();

    public:
        virtual void draw(GraphLib::GraphRenderer *,
                QRectF titleArea);
        void drawVertical(GraphLib::GraphRenderer *,
                          QRectF titleArea);
        void drawHorizontal(GraphLib::GraphRenderer *,
                            QRectF titleArea);

        void setText(const QString &text);
        QString text() const;

        void setTextAlignment(int alignment);
        int textAlignment();

        void setPosition(const QPointF &);
        QPointF getPosition() const;

        void setFont(const QFont &);
        QFont getFont() const;

        void setVisible(bool visible);
        bool isVisible();

    private:
        struct PrivateData;
        PrivateData *m_data;
    };
}


#endif

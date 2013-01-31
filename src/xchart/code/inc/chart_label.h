
#ifndef CHART_LABEL_H_
#define CHART_LABEL_H_

#include "graph_renderer.h"

namespace Cross 
{
    class ChartLabel
    {
        public:
            ChartLabel(QString content);
            ~ChartLabel();

        public:
            static QSizeF size(GraphLib::GraphRenderer *renderer, QString content);

        public:
            void draw(GraphLib::GraphRenderer *renderer,
                      QPointF point);

        private:
            void init();
            void drawBackground(GraphLib::GraphRenderer *renderer,
                                QRectF rect);
            void drawOutline(GraphLib::GraphRenderer *renderer,
                             QRectF rect);
            void drawContent(GraphLib::GraphRenderer *renderer,
                             QPointF point,
                             QString content);


        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}
#endif

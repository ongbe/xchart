
#ifndef _CHART_PIE_PLOT_H_
#define _CHART_PIE_PLOT_H_

#include "chart_plot.h"
#include "chart_pie_dataset.h"

namespace Cross
{

    class ChartPiePlot : public ChartPlot
    {
        public:
            enum ShowStyle
            {
                KEY,
                VALUE,
                PERCENTAGE,
                KEYWITHVALUE,
                KEYWITHRERCENTAGE
            };

            enum LabelStyle
            {
                SIMPLE,
                DOGLEG,
                HIDE
            };

        public: 
            ChartPiePlot(ChartPieDataset *dataset);
            virtual ~ChartPiePlot();

        public:
            ShowStyle showStyle();
            void setShowStyle(ShowStyle showStyle);

            LabelStyle labelStyle();
            void setLabelStyle(LabelStyle style);

            ChartPieDataset *dataset();
            void setDataset(ChartPieDataset *dataset);

            QColor backgroundColor();
            void setBackgroundColor(QColor color);

            bool backgroundIsVisible();
            void setBackgroundVisible(bool visible);

            QColor outlineColor();
            void setOutlineColor(QColor color);

            double outlineWidth();
            void setOutlineWidth(double width);

            bool outlineIsVisible();
            void setOutlineVisible(bool visible);

            double margin();
            void setMargin(double margin);

        public:
            void draw(GraphLib::GraphRenderer *renderer,
                      QRectF plotArea,
                      QPointF anchor);

        private:
            void drawPie(GraphLib::GraphRenderer *renderer,
                      QRectF plotArea,
                      QPointF anchor);

            void drawNoDataMessage();

            void drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect);

            void drawItem(GraphLib::GraphRenderer *renderer,
                         QRectF rect,
                         double startAngle,
                         double spanAngle,
                         QColor color);

            void drawSimpleLabel(GraphLib::GraphRenderer *renderer, QPointF point, QString content);
            void drawLabel(GraphLib::GraphRenderer *renderer,
                           QPointF point,
                           QString content,
                           QVector<GraphLib::Point> pointsDogleg);

        private:
            QColor pieColor(int index);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}


#endif


#ifndef _CHART_XY_PLOT_H_
#define _CHART_XY_PLOT_H_

#include "chart_plot.h"
#include "chart_xy_dataset.h"
#include "chart_number_axis.h"

namespace Cross
{

    class ChartXYPlot : public ChartPlot
    {
        public: 
            ChartXYPlot(ChartXYDataset *dataset);
            virtual ~ChartXYPlot();

        public:
            ChartXYDataset *dataset();
            void setDataset(ChartXYDataset *dataset);

            void setXAxis(ChartNumberAxis *);
            ChartNumberAxis* XAxis();

            void setYAxis(ChartNumberAxis *);
            ChartNumberAxis* YAxis();

            void setXAxisRange(double lower, double upper);
            void setYAxisRange(double lower, double upper);

            void setXGridLineVisible(bool);
            void setYGridLineVisible(bool);

            QColor backgroundColor();
            void setBackgroundColor(QColor color);

            QColor outlineColor();
            void setOutlineColor(QColor color);

            double outlineWidth();
            void setOutlineWidth(double width);

            QColor gridLineColor();
            void setGridLineColor(QColor color);

            double gridLineWidth();
            void setGridLineWidth(double width);

            bool xAxisIsVisible();
            void setXAxisVisible(bool);

            bool yAxisIsVisible();
            void setYAxisVisible(bool);

            bool gridIsVisible();
            void setGridVisible(bool);

            QColor pointColor();
            void setPointColor(QColor color);

            double pointSize();
            void setPointSize(double size);

            bool outlineIsVisible();
            void setOutlineVisible(bool visible);

        public:
            void draw(GraphLib::GraphRenderer *renderer,
                      QRectF plotArea,
                      QPointF anchor);

            void drawXGridLines(GraphLib::GraphRenderer *renderer, QRectF dataArea);
            void drawYGridLines(GraphLib::GraphRenderer *renderer, QRectF dataArea);

            QRectF calculateDataArea(GraphLib::GraphRenderer *renderer, QRectF rect);

        private:
            void drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect);

            void drawPoints(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawPoint(GraphLib::GraphRenderer *renderer, QRectF rect, QVector<QPointF> &);

            void drawAxis(GraphLib::GraphRenderer *renderer, QRectF rect);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}


#endif


#ifndef _CHART_HISTOGRAM_PLOT_H_
#define _CHART_HISTOGRAM_PLOT_H_

#include "chart_plot.h"
#include "chart_axis.h"
#include "chart_histogram_dataset.h"
#include "chart_number_axis.h"
#include "chart_category_axis.h"

namespace Cross
{

    class ChartHistogramPlot : public ChartPlot
    {
        public:
            enum VALUE_SHOW_STYLE {
                VALUE_SHOW_STYLE_HIDE,
                VALUE_SHOW_STYLE_TOP,
                VALUE_SHOW_STYLE_CENTER,
                VALUE_SHOW_STYLE_LEFT
            };

        public: 
            ChartHistogramPlot(ChartHistogramDataset *dataset);
            virtual ~ChartHistogramPlot();

        public:
            ChartHistogramDataset *dataset();
            void setDataset(ChartHistogramDataset *dataset);

            double valueAxisLower();
            void setValueAxisLower(double value);

            double valueAxisUpper();
            void setValueAxisUpper(double value);

            RectangleEdge rectangleEdge();
            void setRectangleEdge(RectangleEdge rectangleEdge);

            bool valueAxisIsVisible();
            void setValueAxisVisible(bool visible);

            bool categoryAxisIsVisible();
            void setCategoryAxisVisible(bool visible);

            double margin();
            void setMargin(double margin);

            double padding();
            void setPadding(double padding);

            double barsPadding();
            void setBarsPadding(double barsPadding);

            VALUE_SHOW_STYLE valueShowStyle();
            void setValueShowStyle(VALUE_SHOW_STYLE valueShowStyle);

            bool gridIsVisible();
            void setGridVisible(bool visible);

            QColor backgroundColor();
            void setBackgroundColor(QColor color);

            bool backgroundIsVisible();
            void setBackgroundVisible(bool visible);

            QColor barsAreaBackgroundColor();
            void setBarsAreaBackgroundColor(QColor color);

            bool barsAreaBackgroundIsVisible();
            void setBarsAreaBackgroundVisible(bool visible);

            QColor barColor();
            void setBarColor(QColor color);


            QColor barOutlineColor();
            void setBarOutlineColor(QColor color);

            double barOutlineWidth();
            void setBarOutlineWidth(double width);


            bool barOutlineIsVisible();
            void setBarOutlineVisible(bool visible);

            bool outlineIsVisible();
            void setOutlineVisible(bool visible);

            bool barsAreaOutlineIsVisible();
            void setBarsAreaOutlineVisible(bool visible);

            QColor outlineColor();
            void setOutlineColor(QColor color);

            double outlineWidth();
            void setOutlineWidth(double width);

            ChartNumberAxis* valueAxis() const;
            ChartCategoryAxis* categoryAxis() const;


            QColor gridLineColor();
            void setGridLineColor(QColor color);

            double gridLineWidth();
            void setGridLineWidth(double width);

        public:
            void draw(GraphLib::GraphRenderer *renderer,
                      QRectF plotArea,
                      QPointF anchor);

        private:
            void drawRectangleEdgeTop(GraphLib::GraphRenderer *renderer,
                                      QRectF plotArea);
            void drawRectangleEdgeLeft(GraphLib::GraphRenderer *renderer,
                                       QRectF plotArea);

            void drawValueAxis(GraphLib::GraphRenderer *renderer,
                           QRectF plotRect,
                           QRectF dataRect,
                           QRectF axisRect,
                           RectangleEdge edge);

            void drawCategoryAxis(GraphLib::GraphRenderer *renderer,
                           QRectF plotRect,
                           QRectF dataRect,
                           QRectF axisRect,
                           RectangleEdge edge);

            void drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect);

            void drawGrid(GraphLib::GraphRenderer *renderer, QRectF rect, RectangleEdge edge);

            void drawBarsAreaBackground(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawBarsAreaOutline(GraphLib::GraphRenderer *renderer, QRectF rect);
            void drawBar(GraphLib::GraphRenderer *renderer,
                         QRectF rect,
                         QString text,
                         VALUE_SHOW_STYLE valueShowStyle);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}


#endif

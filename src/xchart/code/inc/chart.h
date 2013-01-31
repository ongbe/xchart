
#ifndef _CHART_H_
#define _CHART_H_


#include "graph_object.h"
namespace Cross
{
    class GraphLib::GraphRenderer;
    class ChartPlot;

    /**
     * Chart 类库的主要显示容器
     * */
    class Chart : public GraphLib::Object
    {
        public: 
            Chart(ChartPlot *plot = 0);
            virtual ~Chart();

        public:
            QString title();
            void setTitle(QString text);

            bool titleIsVisible();
            void setTitleVisible(bool visible);

            double titleHeight();
            void setTitleHeight(double height);

            bool boundingBoxIsVisible();
            void setBoundingBoxVisibleVisible(bool visible);

            virtual GraphLib::Point pos();
            virtual void setPos(GraphLib::Point);

            double width();
            void setWidth(double width);

            double height();
            void setHeight(double height);

            virtual GraphLib::Rectangle boundingBox();
            virtual GraphLib::Rectangle enclosingBox();

        public:
            void draw(GraphLib::GraphRenderer *renderer);

            virtual void draw(GraphLib::GraphRenderer *renderer,
                    GraphLib::Rectangle chartArea);
        public:
            QRectF plotArea(QRectF chartArea);


        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}


#endif

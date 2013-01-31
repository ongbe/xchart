
#ifndef _GRAPH_VIEW1_H_
#define _GRAPH_VIEW1_H_

#include "graph_geometry.h"
#include "igraph_view.h"

#include <QWidget>


class QMouseEvent;
class QResizeEvent;

namespace GraphLib
{
    class GraphData;
    class GraphRenderer;
    class Doc;
    class View : public QWidget, public IView
    {
        Q_OBJECT
        public:
            View(GraphData *data = 0, QWidget *parent = 0);
            View(Doc *data, QWidget *parent = 0);
            ~View();

            void renderPixmap1(Rectangle *update);

            GraphRenderer *renderer();

        public:
            IDoc *doc();

        public:
            void addUpdatePixels(Point *point, int pixelWidth,
                    int pixelHeight);
            void addUpdateAll();
            void addUpdateWithBorder(Rectangle *rect, int pixelBorder);
            void addUpdate(Rectangle *rect);

        public:
            void flush();

        public slots:
            void selectionChanged(int);


        private slots:
            void hsbUpdate(int);
            void vsbUpdate(int);

        protected:
            void mousePressEvent(QMouseEvent *e);
            void mouseReleaseEvent(QMouseEvent *e);
            void mouseMoveEvent(QMouseEvent *e);

            void paintEvent(QPaintEvent *event);
            void resizeEvent(QResizeEvent *event);

        private:
            void init(Doc *doc, GraphData *data);
            void freeUpdateAreas();

            void setOrigo(int x, int y);
            void updateScrollbars();
        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

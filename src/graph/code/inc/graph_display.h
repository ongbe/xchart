
#ifndef _GRAPH_DISPLAY_H_
#define _GRAPH_DISPLAY_H_

#include "graph_geometry.h"
#include "igraph_view.h"
#include "graph_data.h"
#include "graph_doc.h"

#include <QWidget>

class QMouseEvent;
class QResizeEvent;

namespace GraphLib
{
    class GraphRenderer;
    /**
     * 可以拖动Idoc中的任意对象
     * */

    class Display : public QWidget, public IView
    {
        Q_OBJECT
            
        public:
            enum ModifyToolState
            {
                STATE_NONE,
                STATE_MOVE_OBJECT,
                STATE_MOVE_HANDLE,
                STATE_BOX_SELECT
            };

        public:
            Display(GraphData *data = 0, QWidget *parent = 0);
            Display(IDoc *data, QWidget *parent = 0);
            ~Display();

            void renderPixmap1(Rectangle *update);
            GraphRenderer *renderer();

        public:
            IDoc *doc();
            void flush();

        public:
            void addUpdatePixels(Point *point, int pixelWidth,
                    int pixelHeight);
            void addUpdateAll();
            void addUpdateWithBorder(Rectangle *rect, int pixelBorder);
            void addUpdate(Rectangle *rect);


        public:
            void transformCoords(coord x, coord y,
                    int *xi, int *yi);
            void untransformCoords(coord x, coord y,
                    int *xi, int *yi);

            QPointF transformCoords(QPointF pos);
            QPointF untransformCoords(QPointF pos);

            real transformLength(real dist);
            real untransformLength(real dist);



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
            void keyPressEvent(QKeyEvent *event);

        private:
            void init(IDoc *doc, GraphData *data);
            void freeUpdateAreas();

            void setOrigo(int x, int y);
            void updateScrollbars();

            bool doIfClickedHandle(QPointF *clickedPoint, 
                    QMouseEvent *e);
            Object *clickSelectObject(QPointF *clickedPoint, 
                    QMouseEvent *e);
            std::list<Object *> findSelectedObjects();

        private:
            struct PrivateData;
            struct PrivateData *m_data;
    };
}

#endif

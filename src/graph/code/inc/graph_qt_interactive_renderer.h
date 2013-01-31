
#ifndef _GRAPH_QT_INTERACTIVE_RENDERER_H_
#define _GRAPH_QT_INTERACTIVE_RENDERER_H_

#include "igraph_interactive_renderer.h"
#include "graph_geometry.h"


class QWidget;
class QPaintDevice;

namespace GraphLib
{
    class GraphRenderer;

    class GraphQtInteractiveRenderer : public IGraphInteractiveRenderer
    {
        public:
            GraphQtInteractiveRenderer(GraphRenderer *);
            ~GraphQtInteractiveRenderer();
        public:
             void setSize(void *, int, int);
             void clipRegionClear();
             void clipRegionAddRect(Rectangle *rect);
             void drawPixelLine(int x1, int y1, int x2, int y2, Color *color);
             void drawPixelRect(int x, int y, int width, int height, Color *color);
             void fillPixelRect(int x, int y, int width, int height, Color *color);
             void copyToWindow(void *window, int x, int y, int width, int height);

             /**
              * 可以传进来，QPixmap, QImage, QWidget
              * Fixme: QPainter 会独占QPaintDevice. 两者是唯一对应的，不能
              * 有多个QPainter 关联同一个QPaintDevice.
              * */
             void copyToWindow(QPaintDevice *window, int x, int y, int width, int height);

        private:
             struct PrivateData;
             PrivateData *m_data;
    };
}


#endif

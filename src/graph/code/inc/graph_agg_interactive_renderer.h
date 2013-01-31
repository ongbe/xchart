
#ifndef _GRAPH_AGG_INTERACTIVE_RENDERER_H_
#define _GRAPH_AGG_INTERACTIVE_RENDERER_H_


#include "igraph_interactive_renderer.h"

class QPaintDevice;

namespace GraphLib 
{
    class GraphAggRenderer;
    class GraphAggInteractiveRenderer : public IGraphInteractiveRenderer
    {
        public:
            GraphAggInteractiveRenderer(GraphRenderer *);
            ~GraphAggInteractiveRenderer();

            void copyToWindow(QPaintDevice *device, 
                    int x, int y, int width, int height);

        public:
            void copyToWindow(void *window, int x, int y, int width, int height);
            void setSize(void *window, int w, int h);
            void clipRegionClear();
            void clipRegionAddRect(Rectangle *rect);
            void drawPixelLine(int x1, int y1, int x2, int y2, Color *color);
            void drawPixelRect(int x, int y, int width, int height, Color *color);
            void fillPixelRect(int x, int y, int width, int height, Color *color);

        private:
            struct PrivateData;
            PrivateData *m_data;
           
    };

}


#endif

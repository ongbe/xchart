
#ifndef _GRAPH_INTERACTIVE_RENDERER_H_
#define _GRAPH_INTERACTIVE_RENDERER_H_

#include "graph_geometry.h"
#include "graph_color.h"

namespace GraphLib
{
    class GraphRenderer;
    class IGraphInteractiveRenderer
    {
        public:
            virtual ~IGraphInteractiveRenderer() {};

        public:
            /**
             * /brief 设定Renderer的大小，渲染面板的高度和宽度
             * @param window ,传入renderer的buffer；如果为空, 由内部创建
             * @param w
             * @param h
             * */
            virtual void setSize(void *window, int w, int h) = 0;
            /**
             * /brief 设定Renderer的大小，渲染面板的高度和宽度
             * @param window 接收绘图内容的buffer
             * @param x
             * @param y
             * @param w
             * @param h
             * */
            virtual void copyToWindow(void *window, int x, int y, int width, int height) = 0;
            virtual void clipRegionClear() = 0;
            virtual void clipRegionAddRect(Rectangle *rect) = 0;
            virtual void drawPixelLine(int x1, int y1, int x2, int y2, Color *color) = 0;
            virtual void drawPixelRect(int x, int y, int width, int height, Color *color) = 0;
            virtual void fillPixelRect(int x, int y, int width, int height, Color *color) = 0;
    };
}


#endif

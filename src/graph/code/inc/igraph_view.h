
#ifndef _IGRAPH_VIEW_H_
#define _IGRAPH_VIEW_H_

#include "graph_geometry.h"

#include <QtGlobal>


#define DDISPLAY_MAX_ZOOM 2000.0
#define DDISPLAY_NORMAL_ZOOM 20.0
#define DDISPLAY_MIN_ZOOM 0.2

namespace GraphLib
{
    class IDoc;
    class IView 
    {
        public:
            virtual ~IView() {}

        public:
            virtual IDoc *doc() = 0;

        public:
            /**
             * 更新裁剪区域
             * */
            virtual void addUpdateAll() = 0;
            virtual void addUpdateWithBorder(Rectangle *rect, int pixelBorder) = 0;
            virtual void addUpdate(Rectangle *rect) = 0;
            virtual void addUpdatePixels(Point *point, int pixelWidth,
                    int pixelHeight) = 0;

        public:
            virtual void transformCoordsDouble(coord x, coord y,
                    double *xi, double *yi) 
            {
                Q_UNUSED(x);
                Q_UNUSED(y);
                Q_UNUSED(xi);
                Q_UNUSED(yi);
            }
            virtual void transformCoords(coord x, coord y,
                    int *xi, int *yi) 
            {
                Q_UNUSED(x);
                Q_UNUSED(y);
                Q_UNUSED(xi);
                Q_UNUSED(yi);
            }

            virtual void untransformCoords(coord x, coord y,
                    int *xi, int *yi) 
            {
                Q_UNUSED(x);
                Q_UNUSED(y);
                Q_UNUSED(xi);
                Q_UNUSED(yi);
            }

            virtual real transformLength(real len) 
            {
                return len;
            }
            virtual real untransformLength(real len)
            {
                return len;
            }

            virtual void zoom(Point *point, real zoom_factor)
            {
                Q_UNUSED(point);
                Q_UNUSED(zoom_factor);
            }

            virtual void autoscroll() {}
        public:
            /** 
             * 在修改对象以后更新整个数据集合
             * 对应于QWidget中update();
             * */ 
            virtual void flush() = 0;
    };
}

#endif

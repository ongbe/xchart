
#ifndef _CANVAS_PATH_METHODS_H_
#define _CANVAS_PATH_METHODS_H_

#include <QtGlobal>

namespace GraphLib 
{

    /***
     * interface CanvasPathMethods
     * 绘图路径的接口。
     * 参考： http://dev.w3.org/html5/2dcontext/
     * */
    class ICanvasPathMethods
    {
        public:
            virtual ~ICanvasPathMethods() {}

        public:
            virtual void beginPath() {}
            virtual void endPath() {}

            virtual void closePath() {}
            virtual void moveTo(double /*x*/, double /*y*/) {}
            virtual void lineTo(double /*x*/, double /*y*/) {}

            virtual void arcTo(double /*x1*/, double /*y1*/, 
                    double /*x2*/, double /*y2*/,
                    double /*radius*/) {}
            virtual void rect(double /*x*/, double /*y*/,
                    double /*w*/, double /*h*/) {}
            virtual void arc(double /*x*/, double /*y*/, 
                    double radius,
                    double startAngle, double endAngle,
                    bool anticlockwise) 
            {
                Q_UNUSED(radius)
                Q_UNUSED(startAngle)
                Q_UNUSED(endAngle)
                Q_UNUSED(anticlockwise)
            }

        protected:
            ICanvasPathMethods() {}
    };
}


#endif

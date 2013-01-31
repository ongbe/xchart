

#ifndef _GRAPH_OBJECT_H_
#define _GRAPH_OBJECT_H_

#include "graph_geometry.h"
#include "graph_object_change.h"

namespace GraphLib
{
 
    class IObject 
    {
        public:
            virtual ~Object();

        public:
            /**
             * 绘图对象的中心点，也可以是左上角的点
             * */
            virtual Point pos();
            virtual void setPos(Point );

            /**
             * 绘图对象的外边框和内边框
             * */
            virtual Rectangle boundingBox();
            virtual Rectangle enclosingBox();

            /**
             * 通知IDoc，对象自己发生了改变。
             * */
            virtual void addUpdates(IDoc *doc) {}
            /**
             * 绘制该对象
             * */
            virtual void draw(GraphRenderer *) {}
            /**
             *
             * */
            virtual double distanceFrom(Point *point) {return 0xfffffe;}
            /**
             *
             * @param clickedPoint 鼠标点击的位置
             * @param interactiveRenderer 交互绘制的画笔
             * */
            virtual void selectf(Point *clickedPoint, 
                    GraphRenderer *interactiveRenderer) {}
    };

}


#endif

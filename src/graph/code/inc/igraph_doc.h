
#ifndef _IDOC_H_
#define _IDOC_H_


#include "graph_geometry.h"

#include <list>

namespace GraphLib
{
    class IView;
    class Object;
    class Handle;
    class GraphData;
    class IDoc 
    {
        public:
            virtual ~IDoc() {}

        public:
            /**
             * 更新对象的一组操作函数
             * 
             * */
            //virtual void addUpdateAll() = 0;
            //virtual void addUpdateWithBorder(Rectangle *rect, int pixelBorder) = 0;
            virtual void addUpdate(Rectangle *rect) = 0;
            //virtual void addUpdatePixels(Point *point, int pixelWidth,
                    //int pixelHeight) = 0;

        public:
            virtual Object *findClickedObject(Point *pos, 
                    float maxdist) = 0;
            //double findClosestHandle(Handle **closest, Object **object, Point *pos);


            virtual void select(Object *obj) = 0;
            virtual bool isSelected(Object *obj) = 0;
            virtual void removeAllSelected(int deleteEmpty) = 0;
            /**
             * 选择新一个object。
             * 选中操作会引起selectionChange信号,从而引发view的更新操作
             * */
            virtual void selectList(std::list<Object *> &list) = 0;
            virtual void unselectObject(Object *obj) = 0;
            virtual void unselectObjects(std::list<Object *> &list) = 0;

            /* 对绘图对象的增删改，都会引起界面刷新 */
            virtual void addObject(Object *obj) = 0;
            virtual void addObjectList(std::list<Object *> &list) = 0;
            virtual void removeObject(Object *obj) = 0;

        public:
            virtual void addView(IView *) = 0;
            virtual void removeView(IView *) = 0;

        public:
            /** 
             * 在修改对象以后更新整个数据集合
             * */ 
            virtual void flush() = 0;
            virtual void updateExtents() = 0;

        public:
            /*
             * 提供访问绘图对象数据的接口
             * */
            virtual GraphData *graphData() = 0;
    };
}

#endif

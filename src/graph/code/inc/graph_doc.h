
#ifndef _GRAPH_DOC1_H_
#define _GRAPH_DOC1_H_

#include "graph_geometry.h"
#include "graph_data.h"
#include "igraph_doc.h"

#include <QObject>
#include <list>

namespace GraphLib
{
    class IView;
    class GraphData;
    class Object;
    class Doc : public QObject, public IDoc
    {
        Q_OBJECT

        public:
            Doc(QObject *parent=0);
            virtual ~Doc();

        public:
            /**
             * 更改doc改动的状态信息
             * */
            void setModified(int modified);
            void modified();

        public:
            /**
             * 支持不同的方式，添加clip区域
             * */
            //void addUpdatePixels(Point *point, int pixelWidth, int pixelHeight);
            //void addUpdateAll();
            //void addUpdateWithBorder(Rectangle *rect, int pixelBorder);
            void addUpdate(Rectangle *rect);

        public:
            /**
             * 同一个doc可以连接多个view。
             * */
            void addView(IView *);
            void removeView(IView *);
        public:
            /**
             * 提供访问绘图对象数据的接口
             * */
            GraphData *graphData();
            /**
             * 支持从坐标系中查找一个对象
             * */
            Object *findClickedObject(Point *pos, float maxdist);
            //double findClosestHandle(Handle **closest, Object **object, Point *pos);

            /**
             * 选择新一个object。
             * 选中操作会引起selectionChange信号,从而引发view的更新操作
             * */
            void select(Object *obj);
            void selectList(std::list<Object *> &list);
            void unselectObject(Object *obj);
            void unselectObjects(std::list<Object *> &list);
            bool isSelected(Object *obj);

        public:
            void addObject(Object *obj);
            void addObjectList(std::list<Object *> &list);
            void removeObject(Object *obj);
            void removeAllSelected(int deleteEmpty);


        public:
            void flush();
            void updateExtents() {}


        signals:
            void selectionChanged(int);


        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif

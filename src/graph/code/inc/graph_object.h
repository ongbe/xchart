
#ifndef _GRAPH_OBJECT_H_
#define _GRAPH_OBJECT_H_


#include "graph_geometry.h"
#include "graph_object_change.h"
#include "graph_handle.h"

#include <QTransform>
#include <QString>
#include <QDebug>


namespace GraphLib
{
    class GraphRenderer;
    class Layer;
    class Handle;
    class IDoc;

    /***********************************************
     * 所有图元的基类
     * ********************************************/
    class Object
    {
        public:
            enum {
                Type = 1,
                UserType = 65536
            };

        public:
            virtual ~Object();
            virtual int type() const { return int(Type); }

        public:
            Object *parent();
            void setParent(Object *);

            Layer *parentLayer();
            void setParentLayer(Layer *);

            virtual void setSelectable(bool enable);
            virtual bool isSelectable();
            virtual bool isSelected();

            QString displayName() const;
            void setDisplayName(QString &);

            /**
             * 对父节点的支持
             * */
            void addChild(Object *);
            void removeChild(Object *);
            // 禁止修改该链表中的内容
            std::list<Object *> children();

        public:
            /* 图元的位置 */
            virtual Point pos();
            virtual void setPos(Point );

            /** 图元的外边框和内边框 */
            virtual Rectangle boundingBox();
            virtual Rectangle enclosingBox();

            /** * 返回Handle的链表,每个对象的Handle的个数可以不一样 * */
            virtual std::vector<Handle *> handles();
            /** * 绘制该对象自身，是显示对象的函数 * */
            virtual void draw(GraphRenderer *) = 0;
            /** *  计算与一个点的位置关系 * */
            virtual double distanceFrom(Point *point);
            /**
             *
             * @param clickedPoint 鼠标点击的位置
             * @param interactiveRenderer 交互绘制的画笔
             * */
            virtual void selectf(Point *clickedPoint, 
                    GraphRenderer *interactiveRenderer);
            /** *  将对象自己放在更新区域中 * */
            virtual void addUpdates(IDoc *doc);

            /** * 移动到目标位置 * */
            virtual GraphObjectChange *move(Point *pos);
            /** * 移动handle，从而改变对象的大小或者形状 * */
            virtual GraphObjectChange *moveHandle(Handle *, 
                    Point *pos, HandleMoveReason reason);
            /** * 复制一个对象 * */
            virtual Object *copy();
        public:
            /** * 静态方法，在命名形式上与成员函数区别 * */
            static void object_add_updates_list(std::list<Object *> &, 
                    IDoc *doc);
            static GraphObjectChange *object_list_move_delta(
                    std::list<Object *> &, Point *delta);

        protected:
            Object();

        protected:
            struct PrivateData;
            PrivateData *m_data;


    };

    template <class T> inline T graphobject_cast(Object *item)
    {
        return int(static_cast<T>(0)->Type) == int(Object::Type)
            || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
    }

    template <class T> inline T graphobject_cast(const Object *item)
    {
        return int(static_cast<T>(0)->Type) == int(Object::Type)
            || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
    }


}


#endif

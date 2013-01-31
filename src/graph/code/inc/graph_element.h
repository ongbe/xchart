
#ifndef _GRAPH_ELEMENT_H_
#define _GRAPH_ELEMENT_H_

#include "graph_object.h"
#include "graph_geometry.h"
#include "graph_handle.h"

namespace GraphLib
{
    /**
     * Element表示的是一个被选中以后的如何显示。
     * 本身是一组handle的集合，不同的Object可以定制
     * 显示的选中以后handle的数量和类型.
     *
     * */
    class Element : public Object
    {
        public:
            Element(int numHandles);
            virtual ~Element();

        public:
            virtual Point corner();
            virtual void setCorner(Point pos);

            virtual double width();
            virtual void setWidth(double w);

            virtual double height();
            virtual void setHeight(double h);

        public:
            virtual std::vector<Handle *> handles();

            virtual Rectangle boundingBox();
            virtual Rectangle enclosingBox();
        public:
            virtual void addUpdates(IDoc *doc);


        public:
            /**
             * */
            virtual void updateHandles();
            /**
             *
             * */
            virtual void updateBoundingbox();

            virtual GraphObjectChange * move(Point *to);
            virtual GraphObjectChange *moveHandle(HandleId id,
                                          Point *to,
                                          HandleMoveReason reason);
            /*
            void moveHandleAspect(HandleId id, Point *to, 
                    float aspectRatio);
            void *moveHandle();

            void copy();
            void load();
            void save();
            */

        protected:
            struct PrivateData;
            PrivateData *m_data;
    };
}
#endif

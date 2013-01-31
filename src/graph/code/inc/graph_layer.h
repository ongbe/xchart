
#ifndef _GRAPH_LAYER_H_
#define _GRAPH_LAYER_H_

#include "graph_geometry.h"
#include "graph_color.h"

#include <QObject>
#include <vector>

namespace GraphLib
{
    class GraphData;
    class Object;
    class GraphRenderer;

    class Layer : public QObject
    {
        Q_OBJECT
        public:
            Layer(QObject *parent = 0);
            virtual ~Layer();

        public:
            QString name();
            void setName(QString name);

            Rectangle extents();
            int updateExtents();

            void setVisible(bool );
            bool isVisible();

            GraphData *parent();
            void setParent(GraphData *);

            void objects(std::list<Object *> *objs);

        public:
            void render(GraphRenderer *, Rectangle *update = 0,
                    void *objRenderer = 0, void *data = 0);

            Object *findClosestObjectExcept(Point *pos,
                    float maxdist, void *);
        public:
            void addObject(Object *obj);
            void addObjectAt(Object *obj, int pos);
            void removeObject(Object *obj);

            std::list<Object *> findObjectsInRectangle(Rectangle *rect);
            Object *findClosestObject(Point *pos, float maxdist);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

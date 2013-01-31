
#ifndef _GRAPH_DATA1_H_
#define _GRAPH_DATA1_H_

#include "graph_geometry.h"
#include "graph_color.h"

#include <QObject>
#include <QPointF>
#include <vector>

namespace GraphLib
{
    class Layer;
    class Object;
    class GraphRenderer;
    class Handle;

    class GraphData : public QObject
    {
        Q_OBJECT
        public:
            GraphData();
            virtual ~GraphData();

        signals:
            void objectAdd(GraphData *, Layer *, Object *);
            void objectRemove(GraphData *, Layer *, Object *);

        public:
            Rectangle extents();
            bool updateExtents();
        public:
            std::list<Object *> selectedObjects();

        public:
            /***
             *  对层的操作接口
             * */
            void addLayer(Layer *layer);
            void addLayerAt(Layer *layer, int pos);
            void setActiveLayer(Layer *layer);
            Layer *activeLayer();
            void deleteLayer(Layer *layer);
            int layerGetIndex(const Layer *layer);

            std::vector<Layer *> layers();

            /***
             * 选择obj的接口
             * */
            void select(Object *obj);
            void unselect(Object *obj);
            void removeAllSelected();
            bool isSelected(Object *obj);

        public:
            /***
             * 查找对象的接口
             * */
            Object *findClickedObject(QPointF *pos, double maxdist);
            Object *findClickedObjectExcept(QPointF *pos, 
                    double maxdist,
                    void *avoid);
            double findClosestHandle(Handle **h, 
                    Object **obj, 
                    QPointF *pos);

            void placeUnderSelected();
            void placeOverSelected();
            void placeDownSelected();
            void placeUpSelected();
            void parentSelected();
            void unparentSelected();


        public:
            void render(GraphRenderer *, Rectangle *update = 0,
                    void *objRenderer = 0, void *data = 0);

        private:
            bool computeExtents();
            bool hasVisibleLayers();
            void getLayersExtentsUnion();
        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

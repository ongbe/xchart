
#include "graph_data.h"
#include "graph_object.h"
#include "graph_renderer.h"
#include "graph_layer.h"
#include "graph_handle.h"


#include <QDebug>
#include <algorithm>

namespace GraphLib
{
    static const Rectangle INVALID_EXTENTS = Rectangle(-1.0, -1.0, -1.0, -1.0);
    /**
     *
     *
     *
     * */
    struct GraphData::PrivateData
    {
        Rectangle extents;

        Color bgColor;
        std::vector<Layer *> layers; /* 多个层 **/
        Layer *activeLayer;

        std::list<Object *> selected;
        float scaling;

    };

    GraphData::GraphData() : QObject(0)
    {
        m_data = new PrivateData;
        m_data->extents = INVALID_EXTENTS;
        m_data->selected.clear();

        Layer * l = new Layer();
        m_data->activeLayer = l;
        addLayer(l);
    }

    GraphData::~GraphData()
    {
        delete m_data->activeLayer;
        delete m_data;
    }

    Rectangle GraphData::extents()
    {
        return m_data->extents;
    }

    bool GraphData::updateExtents()
    {
        bool changed = computeExtents();
        return changed;

    }


    void GraphData::getLayersExtentsUnion()
    {
        if (m_data->activeLayer->updateExtents()) {
            m_data->extents = m_data->activeLayer->extents();
        }
    }
    bool GraphData::computeExtents()
    {
        Rectangle oldExtents = m_data->extents;

        if (hasVisibleLayers()) {
            getLayersExtentsUnion();
        } else {
            m_data->extents = INVALID_EXTENTS;
        }
        if (m_data->extents == INVALID_EXTENTS) {
            m_data->extents = Rectangle(0.0, 0.0, 10.0, 10.0);
        }

        return (!(m_data->extents == oldExtents));
    }

    bool GraphData::hasVisibleLayers()
    {
        return true;
    }

    void GraphData::render(GraphRenderer *renderer, Rectangle *update,
            void *objRenderer, void *data)
    {
        Layer *layer;
        int i, len, isActive;

        len = m_data->layers.size();
        for (i = 0; i < len; i++) {
            layer = m_data->layers[i];
            // 活动的层，会有一些特殊的处理
            isActive = (layer == m_data->activeLayer);
            if (layer->isVisible()) {
                layer->render(renderer,
                        update, objRenderer, data);
            }
        }
    }
    Layer *GraphData::activeLayer()
    {
        return m_data->activeLayer;
    }

    std::vector<Layer *> GraphData::layers()
    {
        return m_data->layers;
    }

    struct isEqueal 
    {
        isEqueal(Object *m) : obj(m) {}
        bool operator() (Object *n)
        {
            return obj == n;
        }
        Object *obj;
    };


    void GraphData::select(Object *obj)
    {
        std::list<Object *> &objs = m_data->selected;
        std::list<Object *>::iterator it;


        it = find_if(objs.begin(), objs.end(), isEqueal(obj));
        // find it
        if (it != objs.end())
            return;

        objs.push_back(obj);
    }

    std::list<Object *> GraphData::selectedObjects()
    {
        return m_data->selected;
    }


    void GraphData::unselect(Object *obj)
    {
        if (!obj)
            return;

        std::list<Object *> &objs = m_data->selected;
        std::list<Object *>::iterator it;
        it = std::find_if(objs.begin(), objs.end(), 
                isEqueal(obj));

        //not find
        if (it == objs.end())
            return;

        m_data->selected.remove(obj);
    }

    void GraphData::addLayer(Layer *layer)
    {
        m_data->layers.push_back(layer);
        layer->updateExtents();
        updateExtents();
    }

    // 将layer添加到指定的位置
    void GraphData::addLayerAt(Layer *layer, int pos)
    {
        m_data->layers.push_back(layer);

        int len = m_data->layers.size();

        if ((pos >= 0) && (pos < len)) {
            for (int i = len - 1; i > pos; i--) {
                m_data->layers[i] = m_data->layers[i-1];
            }
            m_data->layers[pos] = layer;
        }

        layer->updateExtents();
        updateExtents();
    }

    void GraphData::setActiveLayer(Layer *layer)
    {
        m_data->activeLayer = layer;
    }

    // 删除一个层，但是至少要保留一个层。
    void GraphData::deleteLayer(Layer *layer)
    {
        int len = m_data->layers.size();
        if (len <= 1)
            return;

        if (m_data->activeLayer == layer) {
            removeAllSelected();
        }

        std::vector<Layer *> &vector = m_data->layers;
        std::vector<Layer *>::iterator it;
        it = std::find(vector.begin(), vector.end(), layer);
        vector.erase(it);

        if (m_data->activeLayer == layer) {
            m_data->activeLayer = m_data->layers.at(0);
        }
    }

    void GraphData::removeAllSelected()
    {
        m_data->selected.clear();
    }

    int GraphData::layerGetIndex(const Layer *layer)
    {
        int i;
        int len;

        len = m_data->layers.size();

        for (i = 0; i < len; ++i) {
            if (layer == m_data->layers[i])
                return i;
        }

        return -1;
    }

    bool GraphData::isSelected(Object *obj)
    {
        std::list<Object *> &objs = m_data->selected;
        std::list<Object *>::iterator it;

        it = find_if(objs.begin(), objs.end(), isEqueal(obj));
        if (it != objs.end())
            return true;

        return false;
    }

    Object *GraphData::findClickedObject(QPointF *pos, 
            double maxdist)
    {
        Layer *l = m_data->activeLayer; 
        return l->findClosestObjectExcept(pos, maxdist, NULL);
    }
    Object *GraphData::findClickedObjectExcept(QPointF *pos, 
            double maxdist,
            void *avoid)
    {
        Layer *l = m_data->activeLayer; 
        return l->findClosestObjectExcept(pos, maxdist, avoid);
    }

    double GraphData::findClosestHandle(Handle **closest, 
            Object **obj, 
            QPointF *pos)
    {
        double mindist = 10000000.0;
        double dist;

        std::list<Object *> &objs = m_data->selected;
        std::list<Object *>::iterator it;

        for (it = objs.begin(); it != objs.end(); ++it) {
             typedef std::vector<Handle *>  Vector;
             typedef std::vector<Handle *>::iterator VIt;

             Vector handles = (*it)->handles();
             VIt handle;
             for (handle = handles.begin(); 
                     handle != handles.end(); handle++) {
                 // 计算距离
                 QPointF postmp = (*handle)->pos();
                 dist = distance_point_point(pos, 
                         &postmp);
                 // 记录最小值
                 if (dist <= mindist) {
                     mindist = dist;
                     *closest = (*handle);
                     *obj = (*it);
                 }
             } //for
        } //for

        return mindist;
    }


}

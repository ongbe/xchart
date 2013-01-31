
#include "graph_doc.h"
#include "graph_data.h"
#include "graph_layer.h"
#include "graph_object.h"
#include "igraph_view.h"

#include <QDebug>

namespace GraphLib
{
    struct Doc::PrivateData
    {
        GraphData *graphData;
        std::list<IView *> displays;
    };

    Doc::Doc(QObject *parent) :
        QObject(parent)
    {
        m_data = new PrivateData;
        m_data->graphData = new GraphData;
    }

    Doc::~Doc()
    {
        delete m_data->graphData;
        delete m_data;
    }

    GraphData *Doc::graphData()
    {
        return m_data->graphData;
    }

    Object *Doc::findClickedObject(Point *pos,
            float maxdist)
    {
        Layer *layer = m_data->graphData->activeLayer();
        return layer->findClosestObjectExcept(pos, maxdist, NULL);
    }

    void Doc::select(Object *obj)
    {
        if (!obj)
            return;

        m_data->graphData->select(obj);
        obj->selectf(NULL, NULL);
        obj->addUpdates(this);
        emit selectionChanged(0);
    }

    void Doc::unselectObject(Object *obj)
    {
        obj->addUpdates(this);
        m_data->graphData->unselect(obj);
        emit selectionChanged(0);
    }

    void Doc::addObject(Object *obj)
    {
        m_data->graphData->activeLayer()->addObject(obj);
    }

    void Doc::removeObject(Object *obj)
    {
        if (obj == NULL)
            return;

        m_data->graphData->activeLayer()->removeObject(obj);
    }

    void Doc::flush()
    {
        typedef std::list<IView *> List;
        typedef std::list<IView *>::iterator It;
        List &list = m_data->displays;
        It it;
        for (it = list.begin(); it != list.end(); it++) {
            (*it)->flush();
        }
    }

    void Doc::addUpdate(Rectangle *update)
    {
        typedef std::list<IView *> List;
        typedef std::list<IView *>::iterator It;
        List &list = m_data->displays;
        It it;
        for (it = list.begin(); it != list.end(); it++) {
            (*it)->addUpdate(update);
        }
    }

    void Doc::addView(IView *view)
    {
        if (view == NULL)
            return;
        m_data->displays.push_back(view);
    }
    void Doc::removeView(IView *view)
    {
        if (view == NULL)
            return;
        m_data->displays.remove(view);
    }

    void Doc::removeAllSelected(int deleteEmpty)
    {
        std::list<Object *> v = m_data->graphData->selectedObjects();
        Object::object_add_updates_list(v, this);
        m_data->graphData->removeAllSelected();
        emit selectionChanged(0);
    }

    bool Doc::isSelected(Object *obj)
    {
        return m_data->graphData->isSelected(obj);
    }
    void Doc::selectList(std::list<Object *> &list)
    {
    }
    void Doc::unselectObjects(std::list<Object *> &list)
    {
    }
    void Doc::addObjectList(std::list<Object *> &list)
    {
    }
}




#include "graph_layer.h"
#include "graph_data.h"
#include "graph_object.h"
#include "graph_geometry.h"

#include <QDebug>
#include <iterator>


namespace GraphLib
{
    static const Rectangle INVALID_EXTENTS = Rectangle(-1.0, -1.0, -1.0, -1.0);
    /**
     *
     *
     *
     *
     * */
    struct Layer::PrivateData
    {
        QString name;
        Rectangle extents;

        std::list<Object *> objects;
        bool visible;

        GraphData *parent;
    };

    Layer::Layer(QObject *p) : QObject(p)
    {
        m_data = new PrivateData;
        m_data->extents = Rectangle(0.0, 0.0, 0.0, 0.0);
        m_data->name = "test";
        m_data->parent = NULL;
        m_data->visible = true;
    }
    Layer::~Layer()
    {
        typedef std::list<Object *> Objs;
        typedef std::list<Object *>::iterator ObjIt;

        Objs &vector = m_data->objects;
        ObjIt it;
        for (it = vector.begin(); it != vector.end(); ++it) {
            Object *obj = *it;
            delete obj;
        }
        vector.clear();

        delete m_data;
    }
    Rectangle Layer::extents()
    {
        return m_data->extents;
    }
    int Layer::updateExtents()
    {

        typedef std::list<Object *> Objs;
        typedef std::list<Object *>::iterator ObjIt;

        Rectangle newExtents = Rectangle(0.0, 0.0, 0.0, 0.0);
        Objs &vector = m_data->objects;
        ObjIt it;
        for (it = vector.begin(); it != vector.end(); ++it) {
            Object *obj = *it;
            Rectangle bbox = obj->boundingBox();
            newExtents = newExtents.united(bbox);
        }
        if (m_data->extents == newExtents)
            return false;

        m_data->extents = newExtents;
        return true;
    }
    QString Layer::name()
    {
        return m_data->name;
    }
    void Layer::setName(QString name)
    {
        m_data->name = name;
    }


    void Layer::setVisible(bool v)
    {
        m_data->visible = v;
    }
    bool Layer::isVisible()
    {
        return m_data->visible;
    }
    GraphData *Layer::parent()
    {
        return m_data->parent;
    }
    void Layer::setParent(GraphData *p)
    {
        m_data->parent = p;
    }


    /**
     * 根据更新区域，绘制图元
     * 如果update == NULL 或者 图元不在更新区域内，那么就不重绘
     * */
    void Layer::render(GraphRenderer *renderer, Rectangle *update,
            void *objRenderer, void *data)
    {
        if (!renderer)
            return;
        typedef std::list<Object *> Objs;
        typedef std::list<Object *>::reverse_iterator ObjIt;

        Objs &vector = m_data->objects;
        ObjIt it;
        for (it = vector.rbegin(); it != vector.rend(); ++it) {
            Object *obj = *it;
            Rectangle rect = obj->boundingBox();

            if (update == NULL || update->intersects(rect)) {
                obj->draw(renderer);
            }
        }
    }
    void Layer::addObject(Object *obj)
    {
        if (!obj)
            return;
        m_data->objects.push_front(obj);
        //qDebug() << "obj size:" << m_data->objects.size();
    }

    Object *Layer::findClosestObjectExcept(Point *pos,
            float maxdist, void *)
    {
        float dist;
        Object *closest = 0;
        
        std::list<Object *> &objects = m_data->objects;
        std::list<Object *>::iterator it;
        for (it = objects.begin(); it != objects.end(); ++it)  {
            dist = (*it)->distanceFrom(pos);
            if (maxdist - dist > 0.00000001) {
                closest = *it;
                break;
            }
        }

        return closest;
    }

    void Layer::removeObject(Object *obj)
    {
        if (obj == NULL)
            return;

        std::list<Object *>::iterator it = m_data->objects.begin();
        while (it != m_data->objects.end()) {
            if (*it == obj) {
                break;
            }
            ++it;
        }
        if (it != m_data->objects.end())
            m_data->objects.erase(it);
    }

    void Layer::objects(std::list<Object *> *objs)
    {
        if (objs == NULL)
            return;

        std::copy(m_data->objects.begin(),
                m_data->objects.end(), std::back_inserter(*objs));
    }

    std::list<Object *> Layer::findObjectsInRectangle(Rectangle *rect)
    {
        std::list<Object *> l;

        typedef std::list<Object *> Objs;
        typedef std::list<Object *>::reverse_iterator ObjIt;

        Objs &vector = m_data->objects;
        ObjIt it;
        for (it = vector.rbegin(); it != vector.rend(); ++it) {
            Object *obj = *it;
            Rectangle bbox = obj->boundingBox();

            if (rect->intersects(bbox)) {
                l.push_front(obj);
            }
        }

        return l;
    }

}


#include "graph_object.h"
#include "graph_geometry.h"
#include "graph_color.h"
#include "graph_object_change.h"
#include "igraph_doc.h"


namespace GraphLib
{
    /**
     * Object 的基本数据
     *
     *
     * */
    struct Object::PrivateData
    {
        Point pos;
        Rectangle boundingBox;
        Rectangle enclosingBox;

        Object *parent;
        std::list<Object *>children;
        int flags;
        std::vector<Handle *> handles;

        Color *highlightColor;

        QString name;
        bool selectable;
    };

    Object::Object()
    {
        m_data = new PrivateData;
        m_data->parent = 0;
        m_data->pos = Point(0, 0);
        m_data->boundingBox = Rectangle(0, 0, 0, 0);
        m_data->enclosingBox = Rectangle(0, 0, 0, 0);
        m_data->selectable = true;
    }

    Object::~Object()
    {
        delete m_data;
    }
    Point Object::pos()
    {
        return m_data->pos;
    }
    void Object::setPos(Point p)
    {
        m_data->pos = p;
    }
    Rectangle Object::boundingBox()
    {
        return m_data->boundingBox;
    }
    Rectangle Object::enclosingBox()
    {
        return m_data->enclosingBox;
    }

    std::vector<Handle *> Object::handles()
    {
        return m_data->handles;
    }

    std::list<Object *> Object::children()
    {
        return m_data->children;
    }

    GraphObjectChange *Object::move(Point *pos) 
    {
        return 0;
    }

    double Object::distanceFrom(Point *)
    {
        return 0xfffffe;
    }

    Object *Object::copy() 
    {
        return NULL; 
    }

    void Object::addUpdates(IDoc *doc)
    {
    }

    void Object::object_add_updates_list(std::list<Object *> &v,
            IDoc *doc)
    {
        if (doc == NULL || v.size() == 0)
            return;

        std::list<Object *>::iterator it;
        for(it = v.begin(); it != v.end(); ++it) {
            Object *obj = (*it);
            obj->addUpdates(doc);
        }
    }


    void Object::removeChild(Object *obj)
    {
        if (obj == NULL)
            return;
        m_data->children.remove(obj);
    }

    GraphObjectChange *Object::moveHandle(Handle *, 
            Point *pos,
            HandleMoveReason reason) 
    { 
        return NULL; 
    }



    void Object::addChild(Object *obj)
    {
        if (obj == NULL)
            return;

        m_data->children.push_back(obj);
    }

    Object *Object::parent()
    {
        return m_data->parent;
    }

    void Object::setParent(Object *p)
    {
        if (m_data->parent == p)
            return;
        if (m_data->parent) {
            m_data->parent->removeChild(this);
            m_data->parent = NULL;
        }
        if (p) {
            p->addChild(this);
        }
        m_data->parent = p;
    }
    GraphObjectChange *Object::object_list_move_delta(
            std::list<Object *> &v, Point *delta)
    {
        std::list<Object *>::iterator it;
        for (it = v.begin(); it != v.end(); ++it) {
            Object *obj = (*it);
            Point pos = obj->pos();
            Point targetPos = pos + *delta;
            obj->move(&targetPos);
        }

        return NULL;
    }
    Layer *Object::parentLayer()
    {
        return NULL;
    }
    void Object::setParentLayer(Layer *)
    {
    }

    void Object::setSelectable(bool enable)
    {
        m_data->selectable = enable;
    }
    bool Object::isSelectable()
    {
        return m_data->selectable;
    }
    bool Object::isSelected()
    {
        return false;
    }

    QString Object::displayName() const
    {
        return m_data->name;
    }

    void Object::setDisplayName(QString &str)
    {
        m_data->name = str;
    }
    void Object::selectf(Point *clickedPoint, 
            GraphRenderer *interactiveRenderer)
    {
    }

}

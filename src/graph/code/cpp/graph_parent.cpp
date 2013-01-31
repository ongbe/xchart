
#include "graph_parent.h"
#include "graph_handle.h"
#include "graph_object.h"

#include <QDebug>


namespace GraphLib
{
    std::list<Object *> parent_list_affected(std::list<Object *> &list)
    {
        std::list<Object *> all_list;

        std::list<Object *>::iterator it;
        for (it = list.begin(); it != list.end(); ++it) {
            all_list.push_back(*it);

            Object *obj = *it;
            std::list<Object *> children = obj->children();
            if (children.size() > 0) {
                std::list<Object *>::iterator child;
                for (child = children.begin(); child != children.end();
                        ++child) {
                    all_list.push_back(*child);
                }
            }
        }

        return all_list;
    }

    bool parent_handle_extents(Object *obj, Rectangle *extents)
    {
        int idx;
        coord *left_most = NULL, *top_most = NULL, *bottom_most = NULL, *right_most = NULL;

        if (obj == NULL)
            return false;

        std::vector<Handle *> v = obj->handles();
        std::vector<Handle *>::iterator it;
        if (v.size() == 0)
            return false;

        // 记录pos的位置
        typedef std::pair<coord, coord> PPoint;
        std::vector< PPoint > buffer;
        for (it = v.begin(); it != v.end(); ++it) {
            Handle *h = (*it);
            Point pos = h->pos();
            buffer.push_back(std::make_pair(pos.x(), pos.y()));
        }

        // 查看最大值，最小值 
        std::vector< PPoint >::iterator bit;
        for (bit = buffer.begin(); bit != buffer.end(); ++bit) {

            if (!left_most || *left_most > bit->first)
                left_most = &bit->first;
            if (!right_most || *right_most < bit->first)
                right_most = &bit->first;
            if (!top_most || *top_most > bit->second)
                top_most = &bit->second;
            if (!bottom_most || *bottom_most < bit->second)
                bottom_most = &bit->second;
        }

        extents->setLeft(*left_most);
        extents->setRight(*right_most);
        extents->setTop(*top_most);
        extents->setBottom(*bottom_most);

        return true;
    }

    bool parent_list_expand(std::list<Object *> &list)
    {
        return false;
    }

    Point parent_move_child_delta(Rectangle *p_ext, 
            Rectangle *c_ext, Point *delta)
    {
        return Point(0, 0);
    }

    bool parent_handle_move_out_check(Object *obj, Point *to)
    {
        Rectangle p_ext, c_ext;
        Point new_delta;
        Object *parent = obj->parent();
        if (parent == NULL)
            return false;
        parent_handle_extents(parent, &p_ext);
        parent_point_extents(to, &c_ext);

        new_delta = parent_move_child_delta(&p_ext, &c_ext, NULL);
        new_delta += *to;

        if (new_delta.x() || new_delta.y())
            return true;

        return false;
    }

    bool parent_handle_move_in_check(Object *, Point *to, 
            Point *start_at)
    {
        return false;
    }

    void parent_point_extents(Point *point, Rectangle *extents)
    {
        extents->setLeft(point->x());
        extents->setRight(point->x());
        extents->setTop(point->y());
        extents->setBottom(point->y());
    }
}





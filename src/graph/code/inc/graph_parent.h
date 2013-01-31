

#ifndef _GRAPH_PARENT_H_
#define _GRAPH_PARENT_H_

#include "graph_object.h"
#include "graph_geometry.h"

#include <QRectF>

namespace GraphLib
{
    std::list<Object *> parent_list_affected(std::list<Object *> &list);

    bool parent_handle_extents(Object *obj, Rectangle *extents);

    bool parent_list_expand(std::list<Object *> &list);

    Point parent_move_child_delta(Rectangle *p_ext, 
            Rectangle *c_ext, Point *delta);

    void parent_point_extents(Point *point, Rectangle *extents);

    bool parent_handle_move_out_check(Object *, Point *to);
    bool parent_handle_move_in_check(Object *, Point *to, 
            Point *start_at);

}


#endif

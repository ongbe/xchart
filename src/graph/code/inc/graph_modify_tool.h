
#ifndef _GRAPH_MODIFY_TOOL_H
#define _GRAPH_MODIFY_TOOL_H


#include "graph_object.h"
#include "graph_data.h"
#include "igraph_doc.h"
#include "graph_doc.h"
#include "igraph_view.h"

#include <QMouseEvent>

namespace GraphLib
{
    class IView;
    class IDoc;
    class ModifyTool
    {
        public:
            enum ModifyToolState
            {
                STATE_NONE,
                STATE_MOVE_OBJECT,
                STATE_MOVE_HANDLE,
                STATE_BOX_SELECT
            };

        public:
            ModifyTool();
            ~ModifyTool();
            int type();

        public:
            void mousePressEvent(QMouseEvent *e);
            void mouseReleaseEvent(QMouseEvent *e);
            void mouseMoveEvent(QMouseEvent *e);

        private:
            bool doIfClickedHandle(QPointF *clickedPoint, 
                    QMouseEvent *e);
            Object *clickSelectObject(QPointF *clickedPoint, 
                    QMouseEvent *e);
            QPointF transformCoords(QPointF pos);
            QPointF untransformCoords(QPointF pos);
            real transformLength(real dist);
            real untransformLength(real dist);



        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

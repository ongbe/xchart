

#include "graph_modify_tool.h"
#include "graph_parent.h"

#include <QMouseEvent>

namespace GraphLib
{
    struct ModifyTool::PrivateData
    {
        GraphData *graphData;
        IDoc *doc;
        IView *view;

        // 选择，移动，改变对象的大小
        ModifyToolState state;
        QPointF lastTo;
        QPointF startAt;
        QPointF origPos;
        GraphLib::Object *object;
        GraphLib::Handle *handle;
    };

    ModifyTool::ModifyTool()
    {
        m_data = new PrivateData;


    }
    ModifyTool::~ModifyTool()
    {
        delete m_data;
    }

    int ModifyTool::type()
    {
        return 0;
    }

    Object *ModifyTool::clickSelectObject(QPointF *clickedPoint, 
            QMouseEvent *e)
    {
        IDoc *doc = m_data->doc;
        Object *obj = NULL;

        obj = doc->findClickedObject(clickedPoint, 1.0);
        if (obj) {
            bool already = doc->isSelected(obj);
            if (!already) {
                doc->removeAllSelected(0);
            }
            doc->select(obj);
            doc->flush();
            return obj;
        }

        return obj;
    }
    bool ModifyTool::doIfClickedHandle(QPointF *clickedPoint, QMouseEvent *e)
    {
        Handle *handle = NULL;
        Object *obj = NULL;
        double dist = m_data->graphData->findClosestHandle(&handle,
                &obj, clickedPoint);

        if (handle && handle->isClicked(NULL, clickedPoint)) {

            m_data->state = STATE_MOVE_HANDLE;
            m_data->lastTo = untransformCoords(handle->pos());
            m_data->handle = handle;
            m_data->object = obj;
            m_data->startAt = untransformCoords(handle->pos());
            return true;
        }
        return false;
    }

    void ModifyTool::mousePressEvent(QMouseEvent *e)
    {
        //QWidget::mousePressEvent(e);
        Object *clickedObj = NULL;

        QPointF clickedPoint = untransformCoords(e->posF());
        IView *view = m_data->view;

        // 点到handle上
        bool someSelected = 
            (m_data->doc->graphData()->selectedObjects().size() > 1);
        if (!someSelected && doIfClickedHandle(&clickedPoint, e))
            return;

        // 点到没有选中的对象上
        clickedObj = clickSelectObject(&clickedPoint, e);
        if (!someSelected && doIfClickedHandle(&clickedPoint, e))
            return;

        // 点到选中的对象上并且不是在handle上
        if (clickedObj) {
            m_data->state = STATE_MOVE_OBJECT;
            m_data->object = clickedObj;
            m_data->startAt = clickedPoint;
            m_data->origPos = clickedObj->pos();
        } else {
            // 点到空白处
            IDoc *doc = m_data->doc;
            doc->removeAllSelected(0);
            doc->flush();
            //update();
        }
    }

    void ModifyTool::mouseReleaseEvent(QMouseEvent *e)
    {
        //QWidget::mouseReleaseEvent(e);
        m_data->state = STATE_NONE;
    }

    void ModifyTool::mouseMoveEvent(QMouseEvent *e)
    {
        //QWidget::mouseMoveEvent(e);

        if (m_data->state == STATE_NONE)
            return;

        // 坐标转换
        QPointF clickedPos = untransformCoords(e->posF());
        IView *view = m_data->view;

        switch(m_data->state) {
            case STATE_MOVE_OBJECT:
                {
                    Object *obj = m_data->object;
                    IDoc *doc = m_data->doc;
                    Handle *handle = m_data->handle;
                    QPointF pos = clickedPos;

                    std::list<Object *> tmp =  
                        m_data->graphData->selectedObjects();
                    std::list<Object *> list = parent_list_affected(
                            tmp);
                    // 当是包含子对象的图元被移动的时候
                    // 子对象也一起移动
                    if (list.size() > 1) {
                        Object::object_add_updates_list(list, doc);
                        //鼠标移动的差值
                        QPointF offset = pos - m_data->startAt;
                        QPointF targetPos = m_data->origPos + offset;
                        //对象移动的差值
                        QPointF delta = targetPos - obj->pos();
                        //qDebug() << delta;
                        Object::object_list_move_delta(list, &delta);
                        Object::object_add_updates_list(list, doc);
                    } else {
                        // 否则，只移动对象本身
                        obj->addUpdates(doc);
                        //鼠标移动的差值
                        QPointF offset = pos - m_data->startAt;
                        QPointF targetPos = m_data->origPos + offset;
                        obj->move(&targetPos);
                        obj->addUpdates(doc);
                    }
                    doc->flush();
                    //update();
                }
                break;
            case STATE_MOVE_HANDLE:
                {
                    Object *obj = m_data->object;
                    IDoc *doc = m_data->doc;
                    Handle *handle = m_data->handle;
                    QPointF pos = clickedPos;

                    obj->addUpdates(doc);
                    obj->moveHandle(handle, &pos,
                            GraphLib::HANDLE_MOVE_USER);
                    obj->addUpdates(doc);
                    doc->flush();
                    //update();
                }
                break;

            case STATE_NONE:
                break;
        };

    }

    QPointF ModifyTool::transformCoords(QPointF pos)
    {
        coord m = pos.x();
        coord n = pos.y();
        int x, y;
        m_data->view->transformCoords(m, n, &x, &y);
        return QPointF(x, y);
    }
    QPointF ModifyTool::untransformCoords(QPointF pos)
    {
        coord m = pos.x();
        coord n = pos.y();
        int x, y;
        m_data->view->untransformCoords(m, n, &x, &y);
        return QPointF(x, y);
    }


    real ModifyTool::transformLength(real dist)
    {
        // dist * zoom_factor;
        return dist;
    }

    real ModifyTool::untransformLength(real dist)
    {
        // dist * zoom_factor;
        return dist;
    }

}


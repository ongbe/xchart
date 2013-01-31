#include "graph_display.h"

#include "graph_renderer.h"
#include "igraph_interactive_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_handle.h"
#include "graph_object.h"
#include "graph_parent.h"
#include "graph_layer.h"
#include "graph_element.h"



#include <QPainter>
#include <QScrollBar>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QTransform>
#include <QtGlobal>
#include <QDebug>

namespace GraphLib
{
    struct Display::PrivateData
    {
        GraphData *graphData;
        IDoc *doc;
        IView *view;

        GraphRenderer *renderer;
        IGraphInteractiveRenderer *iRenderer;
        QScrollBar *hBar;
        QScrollBar *vBar;

        Point origo; /* the origo positon. **/
        float zoomFactor;
        std::list<Rectangle *> updateAreas;
        std::list<Rectangle *> displayAreas;
        Rectangle *visible;


        // 选择，移动，改变对象的大小
        ModifyToolState state;
        QPointF lastTo;
        QPointF startAt;
        QPointF origPos;
        GraphLib::Object *object;
        GraphLib::Handle *handle;

        int x1, y1, x2, y2;
        QPointF startBox;
        QPointF endBox;
    };

    Display::Display(IDoc *data, QWidget *parent) :
        QWidget(parent)
    {
        init(data, data->graphData());
    }
    Display::Display(GraphData *data, QWidget *parent) :
        QWidget(parent)
    {
        init(NULL, data);
    }

    Display::~Display()
    {
        delete m_data->visible;
        delete m_data->iRenderer;
        delete m_data->renderer;
        delete m_data;

    }

    void Display::init(IDoc *doc, GraphData *data)
    {
        m_data = new PrivateData;
        m_data->view = this;

        m_data->visible = new Rectangle;
        m_data->doc = doc;
        if (doc)
            doc->addView(this);
        m_data->graphData = data;

        m_data->renderer = new GraphQtRenderer;
        m_data->iRenderer = new GraphQtInteractiveRenderer(m_data->renderer);
        m_data->iRenderer->setSize(NULL, 2000, 8000);

        m_data->hBar = new QScrollBar(Qt::Horizontal, this);
        connect(m_data->hBar, SIGNAL(valueChanged(int)),
                this, SLOT(hsbUpdate(int)));

        m_data->vBar = new QScrollBar(Qt::Vertical, this);
        connect(m_data->vBar, SIGNAL(valueChanged(int)),
                this, SLOT(vsbUpdate(int)));

        // 左上角起始坐标为0,0点
        setOrigo(0, 0);
        updateScrollbars();
        addUpdateAll();
    }


    void Display::hsbUpdate(int v)
    {
        setOrigo(v, m_data->origo.y());
        addUpdateAll();
        update();
    }
    void Display::vsbUpdate(int v)
    {
        setOrigo(m_data->origo.x(), v);
        addUpdateAll();
        update();
    }
    void Display::addUpdateAll()
    {
        if (m_data->updateAreas.size() > 0)
            freeUpdateAreas();
        addUpdate(m_data->visible);
    }
    void Display::setOrigo(int x, int y)
    {
        Rectangle extents = m_data->graphData->extents();
        Rectangle *visible = m_data->visible;

        // 设定左上角的坐标起始点
        m_data->origo.setX(x);
        m_data->origo.setY(y);

        int width = m_data->renderer->widthPixels();
        int height = m_data->renderer->heightPixels();

        visible->setX(x);
        visible->setY(y);
        visible->setWidth(x + width);
        visible->setHeight(y + height);
    }

    void Display::updateScrollbars()
    {
        Rectangle extents = m_data->graphData->extents();
        Rectangle *visible = m_data->visible;

        m_data->hBar->setMinimum(qMin(visible->left(), extents.left()));
        m_data->hBar->setMaximum(qMax(visible->right(), extents.right()));
        m_data->hBar->setPageStep(visible->right() - visible->left() - 0.0001);
        m_data->hBar->setValue(visible->left());

        m_data->vBar->setMinimum(qMin(visible->top(), extents.top()));
        m_data->vBar->setMaximum(qMax(visible->bottom(), extents.bottom()));
        m_data->vBar->setValue(visible->top());
    }

    void Display::selectionChanged(int)
    {
        update();
    }

    void Display::renderPixmap1(Rectangle *update)
    {
        if (!m_data->graphData)
            return;
        GraphData *data = m_data->graphData;

        // clean background;
        Color color(Qt::white);
        IGraphInteractiveRenderer *iRenderer = m_data->iRenderer;
        GraphRenderer *renderer = m_data->renderer;
        iRenderer->fillPixelRect(0, 0,
                renderer->widthPixels(), renderer->heightPixels(), &color);

        renderer->beginRender();

        data->render(renderer, update, NULL, NULL);

        renderer->endRender();

        // 绘制选中的对象
        typedef std::list<Object *> List;
        typedef std::list<Object *>::iterator It;
        List objs = m_data->graphData->selectedObjects();
        It it;
        for (it = objs.begin(); it != objs.end(); it++) {
            typedef std::vector<Handle *>  Vector;
            typedef std::vector<Handle *>::iterator VIt;
            Vector handles = (*it)->handles();
            VIt handle;
            for (handle = handles.begin(); handle < handles.end(); handle++)
                (*handle)->draw(m_data->renderer);
        }
    }

    GraphRenderer *Display::renderer()
    {
        m_data->renderer;
    }

    void Display::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);

        IGraphInteractiveRenderer *iRenderer = m_data->iRenderer;

        typedef std::list<Rectangle *> List;
        typedef std::list<Rectangle *>::iterator It;
        List &list = m_data->updateAreas;
        It it;
        if (list.size() > 0) {
            it = list.begin();
            Rectangle totrect = *(*it);

            iRenderer->clipRegionClear();
            while (it != list.end()) {
                Rectangle r = *(*it);
                totrect = totrect.united(r);
                iRenderer->clipRegionAddRect(&r);
                it++;
            }
            renderPixmap1(&totrect);
        }
        freeUpdateAreas();

        /*
        areasType &vector1 = m_data->displayAreas;
        it = vector1.begin();
        while (it != vector1.end()) {
            Rectangle r = *it;
            QPixmap pixmap(r.width(), r.height());
            pixmap.fill();
            iRenderer->copyToWindow(&pixmap,
                    r.x(), r.y(), r.width(), r.height());
            painter.drawPixmap(r.topLeft(), pixmap);
        }
        vector1.clear();
        */

        /* 窗口本身还是会存一个和窗口大小相同的buffer */
        QPixmap pixmap(width(), height());
        pixmap.fill();
        m_data->iRenderer->copyToWindow(&pixmap,
                m_data->hBar->value(),
                m_data->vBar->value(), width(), height());

        painter.drawPixmap(QPoint(0, 0), pixmap);


        //m_data->graphData->updateExtents();
        //qDebug() << "extents " << m_data->graphData->extents();
        //painter.drawRect(m_data->graphData->extents());
        //
        if (m_data->state == STATE_BOX_SELECT) {
            QPen pen(Qt::black, 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            painter.drawRect(m_data->x1, m_data->y1,
                    m_data->x2 - m_data->x1, 
                    m_data->y2 - m_data->y1);
        }
    }


    void Display::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);

        QSize size = event->size();
        setOrigo(size.width(), size.height());

        int vBarWidth = m_data->vBar->sizeHint().width();
        int vBarHeight = size.height() - m_data->hBar->sizeHint().height();
        int vBarX = size.width() - vBarWidth;
        m_data->vBar->move(vBarX, 0);
        m_data->vBar->resize(vBarWidth, vBarHeight);
        m_data->vBar->setRange(0, m_data->renderer->heightPixels() - height());

        int hBarHeight = m_data->hBar->sizeHint().height();
        int hBarWidth = size.width() - vBarWidth;
        int hBarY = size.height() - hBarHeight;
        m_data->hBar->move(0, hBarY);
        m_data->hBar->resize(hBarWidth, hBarHeight);
    }
    void Display::addUpdatePixels(Point *point, int pixelWidth,
            int pixelHeight)
    {
        Rectangle rect;
        float sizeX, sizeY;
        sizeX = pixelWidth + 1;
        sizeY = pixelHeight + 1;

        rect.setLeft(point->x() - sizeX/2.0);
        rect.setTop(point->y() - sizeY/2.0);
        rect.setRight(point->x() + sizeX/2.0);
        rect.setBottom(point->y() + sizeY/2.0);

        addUpdate(&rect);
    }

    void Display::addUpdateWithBorder(Rectangle *rect, int pixelBorder)
    {
    }

    void Display::addUpdate(Rectangle *rect)
    {
        Rectangle *r;
        int top, bottom, left, right;
        Rectangle *visible;
        int width, height;

        width = m_data->renderer->widthPixels();
        height = m_data->renderer->heightPixels();

        /*
        if (!m_data->visible.intersects(*rect))
            return;
            */

        typedef std::list<Rectangle *> List;
        typedef std::list<Rectangle *>::iterator It;
        List &list = m_data->updateAreas;
        It it;
        if (list.size() == 0) {
            r = new Rectangle;
            *r = *rect;
            //计算交集后保存
            /*r = m_data->visible.intersected(r);*/
            m_data->updateAreas.push_front(r);
        } else {
            // 先与update中的求并集，然后和visible求交集
            it = list.begin();
            *(*it) = (*it)->united(*rect);
            //*(*it) = (*it)->intersected(m_data->visible);
            //qDebug() << "updateAreas " << *(*it);
        }

        //更新visible
        visible = m_data->visible;

    }

    void Display::freeUpdateAreas()
    {
        typedef std::list<Rectangle *> List;
        typedef std::list<Rectangle *>::iterator It;
        List &list = m_data->updateAreas;
        It it;
        for (it = list.begin(); it != list.end(); it++) {
            Rectangle *r = *it;
            delete r;
        }
        list.clear();
    }

    IDoc *Display::doc()
    {
        return m_data->doc;
    }

    Object *Display::clickSelectObject(QPointF *clickedPoint, 
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

    bool Display::doIfClickedHandle(QPointF *clickedPoint, QMouseEvent *e)
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

    void Display::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);
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
            m_data->state = STATE_BOX_SELECT;
            m_data->startBox = clickedPoint;
            m_data->endBox = clickedPoint;
            m_data->x1 = m_data->x2 =  int(e->posF().x());
            m_data->y1 = m_data->y2 =  int(e->posF().y());
            // drawRect
            /*
            // 点到空白处
            IDoc *doc = m_data->doc;
            doc->removeAllSelected(0);
            doc->flush();
            */
            //update();
        }
    }

    void Display::mouseReleaseEvent(QMouseEvent *e)
    {
        QWidget::mouseReleaseEvent(e);

        switch(m_data->state) {
            case STATE_MOVE_OBJECT:
                m_data->state = STATE_NONE;
                break;
            case STATE_MOVE_HANDLE:
                {
                }
                break;
            case STATE_BOX_SELECT:
                {
                    IDoc *doc = m_data->doc;
                    std::list<Object *> l = findSelectedObjects();
                    if (l.size() > 0) {
                        Object *obj = l.front();
                        bool already = doc->isSelected(obj);
                        if (!already)
                            doc->removeAllSelected(0);
                        doc->select(obj);
                    } else {
                        doc->removeAllSelected(0);
                    }
                    // selectionStyle 分不同的多选类型：
                    // 先实现一个单选的情况
                    m_data->state = STATE_NONE;
                    m_data->doc->flush();
                }
                break;
            case STATE_NONE:
                break;
            default:
                break;
        };

        m_data->state = STATE_NONE;
        //update();
    }

    void Display::mouseMoveEvent(QMouseEvent *e)
    {
        QWidget::mouseMoveEvent(e);

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
                    update();
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
                    update();
                }
                break;
            case STATE_BOX_SELECT:
                {
                    m_data->endBox = e->posF();

                    m_data->x2 = e->posF().x();
                    m_data->y2 = e->posF().y();
                    update();
                }
                break;
            case STATE_NONE:
                break;
        };
    }

    void Display::keyPressEvent(QKeyEvent *event)
    {
        std::list<Object *> objs = m_data->doc->graphData()->selectedObjects();

        if (objs.size() == 0)
            return;
    }

    void Display::transformCoords(coord x, coord y,
            int *xi, int *yi)
    {
        // 还没有用到这个函数
        *xi = qRound(x);
        *yi = qRound(y);
    }

    void Display::untransformCoords(coord x, coord y,
            int *xi, int *yi)
    {
        int m = m_data->hBar->value();
        int n = m_data->vBar->value();
        //qDebug() << x << ":" << y;

        QTransform matrix;
        matrix.translate(m, n);
        coord tmpx = x;
        coord tmpy = y;
        matrix.map(tmpx, tmpy, &x, &y);

        *xi = qRound(x);
        *yi = qRound(y);
    }

    QPointF Display::transformCoords(QPointF pos)
    {
        coord m = pos.x();
        coord n = pos.y();
        int x, y;
        transformCoords(m, n, &x, &y);
        return QPointF(x, y);
    }
    QPointF Display::untransformCoords(QPointF pos)
    {
        coord m = pos.x();
        coord n = pos.y();
        int x, y;
        untransformCoords(m, n, &x, &y);
        return QPointF(x, y);
    }


    real Display::transformLength(real dist)
    {
        // dist * zoom_factor;
        return dist;
    }

    real Display::untransformLength(real dist)
    {
        // dist * zoom_factor;
        return dist;
    }

    void Display::flush()
    {
        update();
    }

    std::list<Object *> Display::findSelectedObjects()
    {
        std::list<Object *> l;

        Rectangle r;
        r.setLeft(qMin(m_data->startBox.x(), m_data->endBox.x()));
        r.setRight(qMax(m_data->startBox.x(), m_data->endBox.x()));
        r.setTop(qMax(m_data->startBox.y(), m_data->endBox.y()));
        r.setBottom(qMin(m_data->startBox.y(), m_data->endBox.y()));

        if (m_data->doc && m_data->doc->graphData()) {
            Layer *layer = m_data->doc->graphData()->activeLayer();
            l = layer->findObjectsInRectangle(&r);
        }

        return l;
    }
}


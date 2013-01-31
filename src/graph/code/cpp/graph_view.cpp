#include "graph_view.h"
#include "graph_renderer.h"
#include "igraph_interactive_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_doc.h"
#include "graph_data.h"
#include "graph_handle.h"
#include "graph_object.h"

#ifdef GRAPH_PATH_RENDERER
//#include "graph_path_renderer.h"
//#include "graph_path_interactive_renderer.h"
#endif

#ifdef GRAPH_AGG_RENDERER
//#include "graph_agg_renderer.h"
//#include "graph_agg_interactive_renderer.h"
#endif



#include <QPainter>
#include <QScrollBar>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QTransform>
#include <QDebug>

namespace GraphLib
{
    struct View::PrivateData 
    {
        GraphData *graphData;
        Doc *doc;

        GraphRenderer *renderer;
        IGraphInteractiveRenderer *iRenderer;
        QScrollBar *hBar;
        QScrollBar *vBar;

        Point origo; /* the origo positon. **/
        float zoomFactor;
        std::list<Rectangle *> updateAreas;
        std::list<Rectangle *> displayAreas;
        Rectangle *visible;
    };

    View::View(Doc *data, QWidget *parent) :
        QWidget(parent)
    {
        init(data, data->graphData());
    }
    View::View(GraphData *data, QWidget *parent) :
        QWidget(parent)
    {
        init(NULL, data);
    }

    View::~View()
    {
        if (m_data->doc) {
            m_data->doc->removeView(this);
        }
        delete m_data->visible;
        delete m_data->iRenderer;
        delete m_data->renderer;
        delete m_data;

    }

    void View::init(Doc *doc, GraphData *data)
    {
        m_data = new PrivateData;
        m_data->visible = new Rectangle;
        m_data->doc = doc;
        if (doc)
            doc->addView(this);

#ifdef GRAPH_PATH_RENDERER
//        m_data->renderer = new GraphPathRenderer();
//        m_data->iRenderer = new GraphPathInteractiveRenderer(m_data->renderer);
//        m_data->iRenderer->setSize(NULL, 1000, 32768);
#elif GRAPH_AGG_RENDERER
//        m_data->renderer = new GraphAggRenderer();
//        m_data->iRenderer = new GraphAggInteractiveRenderer(m_data->renderer);
//        m_data->iRenderer->setSize(NULL, 1000, 32768);
#else 
        m_data->renderer = new GraphQtRenderer;
        m_data->iRenderer = new GraphQtInteractiveRenderer(m_data->renderer); 
        m_data->iRenderer->setSize(NULL, 1000, 32768);
#endif

        m_data->hBar = new QScrollBar(Qt::Horizontal, this);
        connect(m_data->hBar, SIGNAL(valueChanged(int)),
                this, SLOT(hsbUpdate(int)));

        m_data->vBar = new QScrollBar(Qt::Vertical, this);
        connect(m_data->vBar, SIGNAL(valueChanged(int)),
                this, SLOT(vsbUpdate(int)));


        m_data->graphData = data;
        if (m_data->doc) {
            connect(m_data->doc, SIGNAL(selectionChanged(int)),
                    this, SLOT(selectionChanged(int)));
        }

        // 左上角起始坐标为0,0点
        setOrigo(0, 0);
        updateScrollbars();
        addUpdateAll(); 
    }


    void View::hsbUpdate(int v)
    {
        setOrigo(v, m_data->origo.y());
        addUpdateAll();
        update();
    }
    void View::vsbUpdate(int v)
    {
        setOrigo(m_data->origo.x(), v);
        addUpdateAll();
        update();
    }
    void View::addUpdateAll()
    {
        if (m_data->updateAreas.size() > 0)
            freeUpdateAreas();
        addUpdate(m_data->visible);
    }
    void View::setOrigo(int x, int y)
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

    void View::updateScrollbars()
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
    void View::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);

        if (m_data->doc) {
            Point pos(e->pos().x(), e->pos().y());

            QTransform transform;
            transform.translate(m_data->hBar->value(), m_data->vBar->value());
            pos = transform.map(pos);

            Object *obj = m_data->doc->findClickedObject(
                    &pos, 1.0);
            if (obj) {
                m_data->doc->select(obj);
            } else {
                std::list<Object *> already = m_data->doc->graphData()->selectedObjects();
                std::list<Object *>::iterator it;
                for (it = already.begin(); it != already.end(); it++)
                    m_data->doc->unselectObject(*it);
            }
        }
    }

    void View::mouseReleaseEvent(QMouseEvent *e)
    {
        QWidget::mouseReleaseEvent(e);
    }
    void View::mouseMoveEvent(QMouseEvent *e)
    {
        QWidget::mouseMoveEvent(e);
    }

    void View::selectionChanged(int)
    {
        update();
    }

    void View::renderPixmap1(Rectangle *update)
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

#ifdef GRAPH_PATH_RENDERER
        //其中的update参数，是为了优化裁剪。
        //在qt renderer中起作用，在path renderer中使用空
//        data->render(renderer, NULL, NULL, NULL);
#elif GRAPH_AGG_RENDERER
//        data->render(renderer, NULL, NULL, NULL);
#else
        data->render(renderer, update, NULL, NULL);
#endif

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

    GraphRenderer *View::renderer()
    {
        m_data->renderer;
    }

    void View::paintEvent(QPaintEvent *event)
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
    }


    void View::resizeEvent(QResizeEvent *event)
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
    void View::addUpdatePixels(Point *point, int pixelWidth,
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
    void View::addUpdateWithBorder(Rectangle *rect, int pixelBorder)
    {
    }
    void View::addUpdate(Rectangle *rect)
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

    void View::freeUpdateAreas()
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

    IDoc *View::doc()
    {
        return m_data->doc;
    }

    void View::flush()
    {
        update();
    }
}


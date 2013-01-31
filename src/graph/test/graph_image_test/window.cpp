
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_image.h"
#include "graph_handle.h"
#include "graph_doc.h"
#include "graph_view.h"
#include "graph_data.h"
#include "graph_layer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QImage>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphRenderer *renderer;
        GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;
        GraphLib::Doc *doc;
        GraphLib::View *view;
        GraphLib::Handle *handle;
        GraphLib::Image *box;

};

Window::Window(QWidget *p):
    QWidget(p)
{
    init();
}

Window::~Window()
{
    delete m_data->interactiveRenderer;
    delete m_data->renderer;
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 500, 500);

    m_data->renderer->beginRender();

    m_data->doc = new GraphLib::Doc;
    m_data->view = new GraphLib::View(m_data->doc);


    GraphLib::Image *box = new GraphLib::Image;
    m_data->box = box;
    /*
    GraphLib::GraphImage image("test.png");
    box->setImage(image);
    box->setCorner(GraphLib::Point(100.0, 200.0));
    box->setWidth(100.0);
    box->setHeight(100.0);
    box->setBorderWidth(10.0);
    box->setBorderColor(QColor(Qt::red));
    box->setFillColor(QColor(Qt::red));
    */
    QString xml = "<data> \
                   <geometry x=\"100.0\" y=\"100.0\" width=\"300\" height=\"200\" \/> \
                   <image>test.png<\/image> \
                   <\/data>";

    QString property = "<data> \
                   <border>4.0<\/border> \
                   <color>255,0,0<\/color> \
                   <\/data>";
    box->setData(xml);
    box->setProperty(property);
    box->draw(m_data->renderer);
    box->updateHandles();

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(box);

    m_data->renderer->endRender();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_data->view);
    setLayout(layout);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (m_data->handle != NULL)
        return;

    typedef std::vector<GraphLib::Handle *> Vector;
    typedef std::vector<GraphLib::Handle *>::iterator It;
    Vector vector = m_data->box->handles();
    It it;
    for (it = vector.begin(); it != vector.end(); it++) {
        GraphLib::Handle *h = *it;

        QPointF pos(m_data->view->mapFrom(this, event->pos()));

        if (h->isClicked(m_data->view, &pos)) {
            m_data->doc->select(m_data->box);
            m_data->handle = h;
        }
    }
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    m_data->handle = NULL;
    QWidget::mouseReleaseEvent(event);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (event->buttons() && Qt::LeftButton) {
        if (m_data->handle == NULL)
            return;

        QRectF r = rect();

        QPointF pos = event->posF();

        m_data->doc->addUpdate(&r);
        m_data->box->moveHandle(m_data->handle, &pos, GraphLib::HANDLE_MOVE_USER);

        m_data->renderer->beginRender();
        m_data->doc->graphData()->render(m_data->renderer);
        m_data->renderer->endRender();

        repaint();
    }
}

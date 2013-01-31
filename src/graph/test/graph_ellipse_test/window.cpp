
#include "window.h"

#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_ellipse.h"
#include "graph_handle.h"
#include "graph_doc.h"
#include "graph_view.h"
#include "graph_data.h"
#include "graph_layer.h"
#include "graph_object.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphRenderer *renderer;
        GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;

        GraphLib::Doc *doc;
        GraphLib::View *view;

        GraphLib::Ellipse *ellipse;

        GraphLib::Handle *handle;
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

    m_data->handle = NULL;

    m_data->doc = new GraphLib::Doc;
    m_data->view = new GraphLib::View(m_data->doc);

    m_data->renderer = new GraphLib::GraphQtRenderer;

    m_data->interactiveRenderer =
        new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 800, 800);


    QString propertyXml =
            "<property>"
            "   <border-width>0.1</border-width>"
            "   <border-color>#0000ff</border-color>"
            "   <inner-color>#ffff00</inner-color>"
            "   <show-background>1</show-background>"
            "   <line-style>1</line-style>"
            "   <dash-length>1.0</dash-length>"
            "</property>";

    QString dataXml =
            "<data>"
            "   <geometry x='100' y='100' width='100 height='100 />"
            "</data>";


    m_data->ellipse = new GraphLib::Ellipse();
    m_data->ellipse->setData(dataXml);
    m_data->ellipse->setProperty(propertyXml);


    m_data->ellipse->setCorner(GraphLib::Point(250.0, 200.0));
    m_data->ellipse->setWidth(200.0);
    m_data->ellipse->setHeight(100.0);
    m_data->ellipse->updateHandles();

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(m_data->ellipse);

    m_data->renderer->beginRender();

    m_data->ellipse->draw(m_data->renderer);

    m_data->renderer->endRender();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_data->view);
    setLayout(layout);

    resize(800, 600);
}

void Window::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    QPixmap pixmap(width(), height());
    pixmap.fill();
    m_data->interactiveRenderer->copyToWindow(&pixmap, 0, 0, width(), height());

    painter.drawPixmap(0, 0, pixmap);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (m_data->handle != NULL)
        return;

    typedef std::vector<GraphLib::Handle *> Vector;
    typedef std::vector<GraphLib::Handle *>::iterator It;
    Vector vector = m_data->ellipse->handles();
    It it;
    for (it = vector.begin(); it != vector.end(); it++) {
        GraphLib::Handle *h = *it;

        QPointF pos(m_data->view->mapFrom(this, event->pos()));

        if (h->isClicked(m_data->view, &pos)) {
            m_data->doc->select(m_data->ellipse);
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
        m_data->ellipse->moveHandle(m_data->handle, &pos, GraphLib::HANDLE_MOVE_USER);

        m_data->renderer->beginRender();
        m_data->doc->graphData()->render(m_data->renderer);
        m_data->renderer->endRender();

        repaint();
    }
}

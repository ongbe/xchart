
#include "window.h"

#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_textobj.h"
#include "graph_handle.h"
#include "graph_doc.h"
#include "graph_view.h"
#include "graph_data.h"
#include "graph_layer.h"
#include "graph_object.h"
#include "graph_font.h"

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

        GraphLib::Textobj *textobj;

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

    m_data->doc = new GraphLib::Doc;
    m_data->view = new GraphLib::View(m_data->doc);

    m_data->renderer = new GraphLib::GraphQtRenderer;

    m_data->interactiveRenderer =
        new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 800, 800);

    QString propertyXml =
            "<property>"
            "   <font-color>#0000ff</font-color>"
            "   <fill-color>#00ff00</fill-color>"
            "   <alignment>0x0001</alignment>"
            "   <show-background>1</show-background>"
            "   <font-bold>1</font-bold>"
            "   <font-italic>1</font-italic>"
            "   <font-pixelsize>50</font-pixelsize>"
            "</property>";

    QString dataXml =
            "<data>"
            "   <geometry x='100' y='100' text='ABCDEFG' />"
            "</data>";

    m_data->textobj = new GraphLib::Textobj();
    m_data->textobj->setData(dataXml);
    m_data->textobj->setProperty(propertyXml);


    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(m_data->textobj);

    m_data->renderer->beginRender();

    m_data->textobj->draw(m_data->renderer);

    m_data->renderer->endRender();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
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
    Vector vector = m_data->textobj->handles();
    It it;
    for (it = vector.begin(); it != vector.end(); it++) {
        GraphLib::Handle *h = *it;

        QPointF pos(m_data->view->mapFrom(this, event->pos()));

        if (h->isClicked(m_data->view, &pos)) {
            m_data->doc->select(m_data->textobj);
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
        m_data->textobj->moveHandle(m_data->handle, &pos, GraphLib::HANDLE_MOVE_USER);

        m_data->renderer->beginRender();
        m_data->doc->graphData()->render(m_data->renderer);
        m_data->renderer->endRender();

        repaint();
    }
}


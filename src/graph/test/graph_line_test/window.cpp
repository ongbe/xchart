
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"

#include "graph_line.h"
#include "graph_handle.h"

#include "graph_doc.h"
#include "graph_view.h"
#include "graph_data.h"
#include "graph_layer.h"
#include "graph_object.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QBoxLayout>

class Window::PrivateData
{
public:
    GraphLib::Doc *doc;
    GraphLib::View *view;
    GraphLib::Handle *handle;
    GraphLib::Line *line;

    GraphLib::GraphRenderer *renderer;
    GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;
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

    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer =
            new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 800, 800);

//    GraphLib::Line::Parameters parameters;
//    parameters.lineColor = QColor(Qt::black);
//    parameters.lineWidth = 1;
//    parameters.lineStyle = Qt::SolidLine;
//    parameters.startPoint = GraphLib::Point(100,100);
//    parameters.endPoint = GraphLib::Point(300,300);

    QString dataXml;
    QString perXml;

    contentXml(dataXml, perXml);

    m_data->line = new GraphLib::Line;
//    m_data->line->setParameters(parameters);
    m_data->line->setData(dataXml);
    m_data->line->setProperty(perXml);
    m_data->line->updateHandles();

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(m_data->line);

    m_data->renderer->beginRender();
    m_data->line->draw(m_data->renderer);
    m_data->renderer->endRender();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_data->view);
    setLayout(layout);

    resize(500, 500);
}

void Window::contentXml(QString &dataXml, QString &proXml)
{
    proXml =
            "<property>"
            "   <line_width>1</line_width>"
            "   <line_color>#0000ff</line_color>"
            "   <line_style>1</line_style>"
            "   <line_cap>0</line_cap>"
            "   <line_join>0</line_join>"
            "</property>";

    dataXml =
            "<data>"
            "   <start_point x='100' y='100' />"
            "   <end_point x='300' y='300' />"
            "</data>";
}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(width(), height());
    pixmap.fill();
    m_data->interactiveRenderer->copyToWindow(&pixmap, 0, 0, width(), height());

    painter.drawPixmap(500, 500, pixmap);

    QWidget::paintEvent(event);
}


void Window::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (m_data->handle != NULL)
        return;

    QPointF pos = event->posF();
    m_data->line->setClickedPoint(&pos);

    typedef std::vector<GraphLib::Handle *> Vector;
    typedef std::vector<GraphLib::Handle *>::iterator It;
    Vector vector = m_data->line->handles();
    It it;
    for (it = vector.begin(); it != vector.end(); it++) {
        GraphLib::Handle *h = *it;
        QPointF pos(m_data->view->mapFrom(this, event->pos()));

        if (h->isClicked(m_data->view, &pos)) {
            m_data->doc->select(m_data->line);
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
        QRectF r = rect();
        QPointF pos = event->posF();
        m_data->doc->addUpdate(&r);

        if (m_data->handle) {
            m_data->line->moveHandle(m_data->handle, &pos, GraphLib::HANDLE_MOVE_USER);
        } else {
            m_data->line->move(&pos);
        }

        m_data->renderer->beginRender();
        m_data->doc->graphData()->render(m_data->renderer);
        m_data->renderer->endRender();

        repaint();
    }
}

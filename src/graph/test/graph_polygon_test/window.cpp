
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"

#include "graph_polygon.h"
#include "graph_poly_shape.h"
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
#include <QAction>
#include <QMenu>

class Window::PrivateData
{
public:
    QAction *actionAdd;
    QAction *actionDel;

    QPointF clickedPoint;

    GraphLib::Doc *doc;
    GraphLib::View *view;
    GraphLib::Handle *handle;
    GraphLib::Polygon *polygon;

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

    m_data->actionAdd = new QAction(this);
    m_data->actionAdd->setText("添加拐角");

    m_data->actionDel = new QAction(this);
    m_data->actionDel->setText("删除拐角");
    m_data->actionDel->setEnabled(false);

    connect(m_data->actionAdd, SIGNAL(triggered()),
            this, SLOT(addPoint()));
    connect(m_data->actionDel, SIGNAL(triggered()),
            this, SLOT(delPoint()));


    m_data->doc = new GraphLib::Doc;
    m_data->view = new GraphLib::View(m_data->doc);

    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer =
            new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 800, 800);

//    GraphLib::Polygon::Parameters parameters;
//    parameters.lineColor = QColor(Qt::black);
//    parameters.lineWidth = 1;
//    parameters.lineStyle = Qt::SolidLine;

    QString dataXml;
    QString proXml;

    createDataXml(dataXml, proXml);

    m_data->polygon = new GraphLib::Polygon;
//    m_data->polygon->setParameters(parameters);

    m_data->polygon->setData(dataXml);
    m_data->polygon->setProperty(proXml);

    m_data->polygon->updateHandles();

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(m_data->polygon);

    m_data->renderer->beginRender();

    m_data->polygon->draw(m_data->renderer);

    m_data->renderer->endRender();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_data->view);
    setLayout(layout);

    resize(500, 500);
}

void Window::createDataXml(QString &dataXml, QString &proXml)
{
    dataXml =
                "<data>"
                "   <point x='100' y='100'/>"
                "   <point x='100' y='200'/>"
                "   <point x='200' y='200'/>"
                "</data>";
    proXml =
                "<property>"
                "   <line_width>1</line_width>"
                "   <line_color>#0000ff</line_color>"
                "   <fill_color>#00ff00</fill_color>"
                "   <show_background>1</show_background>"
                "   <line_style>1</line_style>"
                "</property>";
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

    QPointF pos = event->posF();
    m_data->clickedPoint = pos;

    if (m_data->handle != NULL)
        return;

    m_data->polygon->setClickedPoint(&pos);

    typedef std::vector<GraphLib::Handle *> Vector;
    typedef std::vector<GraphLib::Handle *>::iterator It;
    Vector vector = m_data->polygon->handles();
    It it;
    for (it = vector.begin(); it != vector.end(); it++) {
        GraphLib::Handle *h = *it;
        QPointF pos(m_data->view->mapFrom(this, event->pos()));

        if (h->isClicked(m_data->view, &pos)) {
            m_data->doc->select(m_data->polygon);
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
            m_data->polygon->moveHandle(m_data->handle, &pos, GraphLib::HANDLE_MOVE_USER);
        } else {
            m_data->polygon->move(&pos);
        }

        m_data->renderer->beginRender();
        m_data->doc->graphData()->render(m_data->renderer);
        m_data->renderer->endRender();

        repaint();
    }
}

void Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;

    menu.addAction(m_data->actionAdd);
    menu.addAction(m_data->actionDel);

    menu.exec(QCursor::pos());
}

void Window::addPoint()
{
    m_data->polygon->addPoint(m_data->clickedPoint);
    if (m_data->polygon->pointSize() > 3)
        m_data->actionDel->setEnabled(true);

    QRectF r = rect();
    m_data->doc->addUpdate(&r);

    m_data->renderer->beginRender();
    m_data->doc->graphData()->render(m_data->renderer);
    m_data->renderer->endRender();

    repaint();
}

void Window::delPoint()
{
    m_data->polygon->removePoint(m_data->clickedPoint);
    if (m_data->polygon->pointSize() < 4)
        m_data->actionDel->setEnabled(false);

    QRectF r = rect();
    m_data->doc->addUpdate(&r);

    m_data->renderer->beginRender();
    m_data->doc->graphData()->render(m_data->renderer);
    m_data->renderer->endRender();

    repaint();
}

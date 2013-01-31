
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_polyline.h"
#include "graph_handle.h"
#include "graph_view.h"
#include "graph_doc.h"
#include "graph_data.h"
#include "graph_layer.h"
#include "graph_handle.h"

#include <QPainter>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphRenderer *renderer;
        GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;
        GraphLib::Doc *doc;
        GraphLib::View *view;
        GraphLib::Polyline *line;
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

    m_data->doc = new GraphLib::Doc;
    m_data->view = new GraphLib::View(m_data->doc);

    m_data->renderer->beginRender();

    GraphLib::Polyline *box = new GraphLib::Polyline;
    m_data->line = box;
    QString xml = "<data> \
                   <geometry x=\"50.0\" y=\"50.0\"\/> \
                   <geometry x=\"100.0\" y=\"100.0\"\/> \
                   <geometry x=\"100.0\" y=\"200.0\"\/> \
                   <\/data>";

    QString property = "<data> \
                   <border>4.0<\/border> \
                   <color>255,0,0<\/color> \
                   <\/data>";
    box->setData(xml);
    box->setProperty(property);

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(box);

    box->setDoc(m_data->doc);
    box->draw(m_data->renderer);

    QPushButton *btn = new QPushButton("add");
    connect(btn, 
            SIGNAL(clicked()),
            this,
            SLOT(add()));

    QPushButton *btn1 = new QPushButton("remove");
    connect(btn1, 
            SIGNAL(clicked()),
            this,
            SLOT(remove()));

    QPushButton *btn2 = new QPushButton("move");
    connect(btn2, 
            SIGNAL(clicked()),
            this,
            SLOT(moveline()));

    QPushButton *btn3 = new QPushButton("copy");
    connect(btn3, 
            SIGNAL(clicked()),
            this,
            SLOT(copy()));








    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(btn);
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    layout->addWidget(btn3);
    layout->addWidget(m_data->view);
    //layout->addStretch(0);
    setLayout(layout);
    m_data->renderer->endRender();
}

void Window::add()
{
    GraphLib::Point p(50, 80);
    m_data->line->addPoint(p);
    m_data->line->addUpdates(m_data->doc);
    GraphLib::GraphData *data = m_data->doc->graphData();
    data->render(m_data->renderer);
    m_data->doc->flush();
    m_data->view->update();
}

void Window::remove()
{
    GraphLib::Point p(0, 0);
    m_data->line->removePoint(p);
    m_data->line->addUpdates(m_data->doc);
    GraphLib::GraphData *data = m_data->doc->graphData();
    data->render(m_data->renderer);
    m_data->doc->flush();
    m_data->view->update();
}

void Window::moveline()
{
    std::vector<GraphLib::Handle *> handles = m_data->line->handles();
    GraphLib::Handle *h = handles[2];
    GraphLib::Point point(200, 250);
    m_data->line->moveHandle(h, &point, GraphLib::HANDLE_MOVE_USER);
    m_data->line->addUpdates(m_data->doc);
    GraphLib::GraphData *data = m_data->doc->graphData();
    data->render(m_data->renderer);
    m_data->doc->flush();
    m_data->view->update();

}

void Window::copy()
{
    GraphLib::Object *box = m_data->line->copy();
    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(box);
}

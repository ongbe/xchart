
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_handle.h"
#include "graph_geometry.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphQtRenderer *renderer;
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
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer((GraphLib::GraphRenderer *)m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 10000, 10000);

    resize(800, 600);
    qDebug() << "init";


    m_data->renderer->beginRender();

    GraphLib::Handle handle;
    handle.setPos(GraphLib::Point(30, 30));
    handle.draw(m_data->renderer);

    handle.setPos(GraphLib::Point(30, 60));
    handle.setType(GraphLib::HANDLE_MAJOR_CONTROL);
    handle.draw(m_data->renderer);
    
    handle.setPos(GraphLib::Point(30, 90));
    handle.setType(GraphLib::HANDLE_MINOR_CONTROL);
    handle.draw(m_data->renderer);



    //delete m_data->renderer;

    m_data->renderer->endRender();


}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    m_data->interactiveRenderer->copyToWindow(this, 0, 0, width(), height());
}

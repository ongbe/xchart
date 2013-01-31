
#include "window.h"
#include "graph_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"

#ifdef GRAPH_PATH_RENDERER
#include "graph_path_renderer.h"
#include "graph_path_interactive_renderer.h"
#else
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#endif

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphRenderer *renderer;
        GraphLib::IGraphInteractiveRenderer *interactiveRenderer;
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

#ifdef GRAPH_PATH_RENDERER
    m_data->renderer = new GraphLib::GraphPathRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphPathInteractiveRenderer(m_data->renderer);
    //m_data->interactiveRenderer->setSize(NULL, 500, 500);
#else
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 500, 500);
#endif

    resize(800, 600);
    qDebug() << "init";

    m_data->renderer->beginRender();

    GraphLib::Box box1;
    GraphLib::Point corner(200.0, 200.0);
    box1.setCorner(corner);
    box1.setBorderWidth(5.0);
    box1.setFillColor(QColor(Qt::green));
    box1.setBackgroundVisible(true);
    box1.setBorderStyle(Qt::DotLine);
    /*
       box1.setWidth(100.0);
       box1.setHeight(100.0);
       box1.setBorderWidth(3.0);
       box1.setCorner(GraphLib::Point(200.0, 200.0));
       box1.setInnerColor(new QColor(Qt::white));
       */
    box1.draw(m_data->renderer);



    GraphLib::Box box;
    box.setWidth(100.0);
    box.setHeight(100.0);
    box.setCorner(GraphLib::Point(20.0, 20.0));
    box.setFillColor(QColor(Qt::red));
    box.draw(m_data->renderer);

    box.updateHandles();

    std::vector<GraphLib::Handle *> handles = box.handles();
    qDebug() << handles.size();
    for (int i = 0; i < handles.size(); ++i) {
        GraphLib::Handle *it = handles.at(i);
        it->draw(m_data->renderer);
    }

    m_data->renderer->endRender();

}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(width(), height());
    pixmap.fill();
    m_data->interactiveRenderer->copyToWindow(&pixmap, 0, 0, width(), height());

    painter.drawPixmap(0, 0, pixmap);

}
void Window::keyPressEvent(QKeyEvent *event)
{

    switch(event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        default:
            QWidget::keyPressEvent(event);
    };

}

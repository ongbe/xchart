
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
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
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 500, 500);

    m_data->renderer->beginRender();

    GraphLib::Box box;
    box.setCorner(GraphLib::Point(100.0, 200.0));
    box.setWidth(100.0);
    box.setHeight(100.0);
    box.setBorderWidth(10.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));
    box.draw(m_data->renderer);
    box.updateHandles();

    GraphLib::Box box1;
    GraphLib::Point corner(200.0, 200.0);
    box1.setCorner(corner);
    box1.setBorderWidth(5.0);
    //box1.setFillColor(QColor(Qt::green));
    //box1.setBackgroundVisible(true);
    //box1.setBorderStyle(Qt::DotLine);
    box1.draw(m_data->renderer);

    /*
    std::vector<GraphLib::Handle *> handles = box.handles();
    for (int i = 0; i < handles.size(); ++i) {
        GraphLib::Handle *it = handles.at(i);
        it->draw(m_data->renderer);
    }
    */


    /*
    GraphLib::Point start(300, 40);
    GraphLib::Point end(350, 85);
    GraphLib::Color color(0, 255, 255);
    //m_data->renderer->drawLine(&start, &end, &color);
    m_data->renderer->fillRect(&start, &end, &color);

    GraphLib::Point start1(300, 80);
    GraphLib::Point end1(350, 90);
    GraphLib::Color color1(0, 0, 0);
    //m_data->renderer->drawLine(&start, &end, &color);
    m_data->renderer->fillRect(&start1, &end1, &color1);

    GraphLib::Point start2(400, 80);
    GraphLib::Point end2(450, 90);
    GraphLib::Color color2(0, 0, 0);
    //m_data->renderer->drawLine(&start, &end, &color);
    m_data->renderer->fillRect(&start2, &end2, &color2);
    */





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

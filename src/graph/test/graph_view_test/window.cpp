
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_view.h"
#include "graph_data.h"
#include "graph_doc.h"
#include "graph_layer.h"
#include "graph_box.h"

#include <QKeyEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QLabel>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::View *view;
        GraphLib::Doc *doc;
};

Window::Window(QWidget *p):
    QWidget(p)
{
    init();
}

Window::~Window()
{
    delete m_data->view;
    delete m_data->doc;
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;

    GraphLib::Box *box = new GraphLib::Box;
    box->setCorner(GraphLib::Point(100.0, 100.0));
    box->setWidth(300.0);
    box->setHeight(500.0);

    GraphLib::Box *box1 = new GraphLib::Box;
    box1->setCorner(GraphLib::Point(200.0, 500.0));
    box1->setWidth(100.0);
    box1->setHeight(50.0);

    GraphLib::Box *box2 = new GraphLib::Box;
    box2->setCorner(GraphLib::Point(200.0, 1000.0));
    box2->setWidth(100.0);
    box2->setHeight(50.0);

    m_data->doc = new GraphLib::Doc;

    GraphLib::GraphData *data = m_data->doc->graphData();
    data->activeLayer()->addObject(box);
    data->activeLayer()->addObject(box1);
    data->activeLayer()->addObject(box2);


    m_data->view = new GraphLib::View(m_data->doc);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_data->view);
    setLayout(layout);


    resize(800, 600);
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


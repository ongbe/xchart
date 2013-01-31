
#include "window.h"
#include "drawable.h"
#include "tile_types.h"
#include "tile_manager.h"
#include "tile.h"
#include "tile_swap.h"

#include "graph_data.h"
#include "graph_box.h"
#include "graph_layer.h"
#include "graph_renderer.h"
#include "graph_path_interactive_renderer.h"
#include "graph_path_renderer.h"


#include <QPainter>
#include <QKeyEvent>
#include <QImage>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::Drawable *d;
};

Window::Window(QWidget *p):
    QWidget(p)
{
    init();
}

Window::~Window()
{
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;
    m_data->d = new GraphLib::Drawable;
    GraphLib::TileManager *tm = m_data->d->get_tiles();

    // 载入测试数据
    GraphLib::GraphData dd;
    qDebug() << "没有数据的时候， ++ extents: " << dd.extents();

    GraphLib::Box *box1 = new GraphLib::Box;
    GraphLib::Box &box = *box1;
    box.setCorner(GraphLib::Point(10.0, 20.0));
    box.setWidth(200.0);
    box.setHeight(200.0);
    box.setBorderWidth(3.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));

    dd.activeLayer()->addObject(&box);
    if (dd.updateExtents()) {
        qDebug() << "++ layer extents: " << dd.activeLayer()->extents();
        qDebug() << "++ data extents: " << dd.extents();
    } 
    GraphLib::Rectangle extent = dd.extents();

    GraphLib::GraphPathRenderer *renderer;
    GraphLib::GraphPathInteractiveRenderer *irenderer;
    renderer = new GraphLib::GraphPathRenderer;

    // 在render的过程中，形成path
    dd.render(renderer);

    // 在tile上绘制图形
    m_data->d->setData(&dd, renderer);
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
void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(300, 300);
    pixmap.fill();
    m_data->d->copyToWindow(&pixmap, 0, 0, 220, 220);

    painter.drawPixmap(0, 0, pixmap);
    
    return QWidget::paintEvent(event);

}



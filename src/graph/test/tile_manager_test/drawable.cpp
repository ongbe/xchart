
#include "drawable.h"
#include "tile_types.h"
#include "tile_manager.h"
#include "tile_manager.h"
#include "tile.h"
#include "graph_path_renderer.h"
#include "graph_data.h"

#include <QImage>
#include <QTransform>
#include <QPaintDevice>
#include <QPainter>
#include <QRectF>
#include <QDebug>

namespace GraphLib
{
    struct Drawable::PrivateData 
    {
        TileManager *tiles; 
    };

    Drawable::Drawable(QString name, 
            int offset_x,
            int offset_y, 
            int width, 
            int height)
    {
        m_data = new PrivateData;
        m_data->tiles = 0; 
    }
    Drawable::~Drawable()
    {
        if (m_data->tiles) {
            delete m_data->tiles;
        }

        delete m_data;
    }

    TileManager *Drawable::get_tiles()
    {
        return m_data->tiles;
    }

    void Drawable::update(int x, int y, int width, int height)
    {
    }

    void Drawable::setData(GraphData *data,GraphPathRenderer *renderer)
    {
        QRectF extent = data->extents();
        qDebug() << "GRAPH DATA extent: " << extent;
        if (!m_data->tiles) {
            m_data->tiles = new TileManager(extent.width() + 10, 
                    extent.height() + 10, 4);
        }

        int ntile_rows = m_data->tiles->ntile_rows;
        int ntile_cols = m_data->tiles->ntile_cols;

        GraphLib::TileManager *tm = m_data->tiles;
        for (int i = 0; i < ntile_cols; ++i) {

            for (int j = 0; j < ntile_rows; ++j) {

                GraphLib::Tile *t = tm->get_at(i, j, true, true);
                QImage image(t->data, t->ewidth, t->eheight,
                        QImage::Format_ARGB32_Premultiplied);

                int w = t->ewidth;
                int h = t->eheight;

                QRectF r1;
                QTransform t1;

                //graph 移动到自己的中心点
                t1.translate(-1*extent.center().x(), -1*extent.center().y());
                //移动到图像的中心点
                t1.translate(tm->width/2, tm->height/2);
                //设置存储介质和坐标转换器
                t1.translate(-1 * i * TILE_WIDTH, -1 * j * TILE_HEIGHT);

                renderer->render(&image, r1, t1);

                //qDebug() << i << ":" << j << "demo.jpg";
                // 每个tile在绘制以后，就不再使用了。
                Tile::release(t, true);
            }
        }
    }


    void Drawable::copyToWindow(QPaintDevice *device, int x, int y,
            int width, int height)
    {
        QPainter painter(device);

        GraphLib::TileManager *tm = m_data->tiles;

        int rows = height / TILE_HEIGHT + 1;
        int cols = width / TILE_WIDTH + 1;

        QTransform t;
        t.translate(-1 * x, -1 * y);
        painter.setTransform(t);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {

                GraphLib::Tile *t = tm->get_tile(
                        x + j*TILE_WIDTH, 
                        y + i*TILE_HEIGHT, true, true);
                if (t) {
                    int tile_x, tile_y;
                    tm->get_tile_coordinates(t, &tile_x, &tile_y);

                    qDebug() << t->tlink->tile_num;

                    QImage image(t->data, t->ewidth, t->eheight,
                            QImage::Format_ARGB32_Premultiplied);

                    int w = t->ewidth;
                    int h = t->eheight;

                    QRectF targetRect(tile_x, tile_y, w, h);
                    QRectF sourceRect(0, 0, w, h);

                    painter.drawRect(QRectF(tile_x, tile_y, w, h));
                    painter.drawImage(targetRect, image, sourceRect);
                }
            }
        }
    }
}



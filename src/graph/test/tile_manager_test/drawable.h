

#ifndef _DRAWABLE_H_
#define  _DRAWABLE_H_


#include "tile_types.h"

#include <QString>

class QPaintDevice;
namespace GraphLib
{
    class GraphPathRenderer;
    class GraphData;
    /**
     * 一个使用tile manager的类，作为graph data和 tile manager的中介。
     *
     * */

    class Drawable
    {
        public:
            Drawable(QString name = "drawable", 
                    int offset_x = 0,
                    int offset_y = 0, 
                    int width = 200, 
                    int height = 200);
            virtual ~Drawable();

            // 存放需要绘制的内容
            void setData(GraphData *, GraphPathRenderer *renderer);

            TileManager *get_tiles();
            void update(int x, int y, int width, int height);


            // 根据给定的矩形，显示图片。
            void copyToWindow(QPaintDevice *, int x, int y,
                    int width, int height);
        private:
            struct PrivateData; 
            PrivateData *m_data;
    };
}

#endif

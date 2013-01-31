

#ifndef _GRAPH_TEXT_H_ 
#define _GRAPH_TEXT_H_

#include "graph_geometry.h"
#include "graph_color.h"


class QString;
namespace GraphLib
{
    /**
     * 支持多行显示文本
     * 识别\n分行的字符串
     * */
    class GraphRenderer;
    class GraphFont;
    class GraphText
    {
        public:
            GraphText(const QString *string, GraphFont *font,
                    float height, Point *pos, Color *color, 
                    Alignment align);
            ~GraphText();

        public:
            void draw(GraphRenderer *);
            GraphText *copy();

        private:
            void setString(const QString *str);


        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif

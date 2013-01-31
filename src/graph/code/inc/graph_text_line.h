

#ifndef _GRAPH_TEXT_LINE_H_ 
#define _GRAPH_TEXT_LINE_H_


class QString;
namespace GraphLib
{
    /**
     * 支持多行显示文本
     *
     * */
    class GraphFont;
    class GraphTextLine
    {
        public:
            explicit GraphTextLine(QString *text=0, 
                    GraphFont *font=0, float fontHeight=0);
            ~GraphTextLine();

        public:
            GraphTextLine *copy();

            void setString(QString *string);
            QString *string();

            void setFont(GraphFont *font);
            GraphFont *font();

            void setHeight(float height);
            float height();
            float width();

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif

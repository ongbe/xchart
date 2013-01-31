
#ifndef _GRAPH_FONT_H_
#define _GRAPH_FONT_H_

#include <QString>
#include <QFont>

namespace GraphLib
{
    /**
     * 字体类，提供字体信息
     * */
    class GraphFont : public QFont
    {
        public:
            GraphFont();
            ~GraphFont();
        public:
            float height();
            float stringWidth(QString &str);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif

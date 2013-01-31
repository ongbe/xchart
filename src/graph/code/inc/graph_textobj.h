
#ifndef _GRAPH_TEXTOBJ_H_
#define _GRAPH_TEXTOBJ_H_

#include "graph_element.h"
#include "graph_renderer.h"

namespace GraphLib
{
    class Textobj : public Element
    {
        public:
            Textobj();
            ~Textobj();

        public:
            QString text();
            void setText(QString text);

            GraphLib::GraphFont *font();
            void setFont(GraphLib::GraphFont *font);

            GraphLib::Color fontColor();
            void setFontColor(GraphLib::Color color);

            GraphLib::Color fillColor();
            void setFillColor(GraphLib::Color color);

            GraphLib::Alignment alignment();
            void setAlignment(GraphLib::Alignment alignment);

            bool showBackground();
            void setShowBackground(bool show);

        public:
            void setData(QString &xml);
            void setProperty(QString &xml);

        public:
            void updateHandles();
            std::vector<Handle *> handles();
            GraphObjectChange * move(Point *to);
            GraphObjectChange *moveHandle(Handle *handle,
                                          Point *to,
                                          HandleMoveReason reason);

        public:
            GraphLib::Object *copy();

        public:
            Rectangle boundingBox();
            Rectangle enclosingBox();

            void draw(GraphRenderer *renderer);
            void selectf(Point *clickedPoint, GraphRenderer *interactiveRenderer);
            double distanceFrom(Point *point);

        private:
            void setString(const QString *str);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

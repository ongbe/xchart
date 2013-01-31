
#include "graph_handle.h"
#include "graph_color.h"
#include "graph_renderer.h"
#include "igraph_doc.h"
#include "igraph_interactive_renderer.h"
#include "graph_qt_interactive_renderer.h"

#include <QDebug>

#define HANDLE_SIZE 7.0

namespace GraphLib
{
    static const Color handleColor[NUM_HANDLE_TYPES<<1] =
    {
        { 0, 0, 127}, /* HANDLE_NON_MOVABLE */
        { 0, 255, 0}, /* HANDLE_MAJOR_CONTROL */
        { 255, 153, 0}, /* HANDLE_MINOR_CONTROL */
        /* dim down the color if the handle is in a group of selected objects */
        { 0, 0, 127}, /* HANDLE_NON_MOVABLE */
        { 0, 178, 0}, /* HANDLE_MAJOR_CONTROL */
        { 178, 102, 0}, /* HANDLE_MINOR_CONTROL */
    };

    struct Handle::PrivateData
    {
        HandleId id; /*!< gives (mostly) the placement relative to the object */
        HandleType type; /*!< color coding */
        Point pos;   /*! where the handle currently is in diagram coordinates */
    };

    Handle::Handle()
    {
        m_data = new PrivateData;
        m_data->pos = Point(0, 0);
        m_data->type = HANDLE_NON_MOVABLE;
    }
    Handle::~Handle()
    {
        delete m_data;
    }

    Point Handle::pos()
    {
        return m_data->pos;
    }
    void Handle::setPos(Point pos)
    {
        m_data->pos = pos;
    }
    HandleId Handle::id()
    {
        return m_data->id;
    }
    void Handle::setId(HandleId id)
    {
        m_data->id = id;
    }
    HandleType Handle::type()
    {
        return m_data->type;
    }
    void Handle::setType(HandleType type)
    {
        m_data->type = type;
    }

    void Handle::draw(GraphRenderer *renderer)
    {
        if (!renderer)
            return;

        IGraphInteractiveRenderer *irenderer = 
            renderer->interactiveRenderer();
        if (!irenderer)
            return;

        int x, y;
        bool someSelected = false;

        // 平移坐标
        x = m_data->pos.x();
        y = m_data->pos.y();
        const Color *color = 
            &handleColor[m_data->type + (someSelected ? NUM_HANDLE_TYPES : 0)];

        irenderer->fillPixelRect(x - HANDLE_SIZE/2.0 + 1,
                y - HANDLE_SIZE/2.0 + 1,
                HANDLE_SIZE - 2.0, HANDLE_SIZE - 2,
                (Color *)color);

        irenderer->drawPixelRect(x - HANDLE_SIZE/2.0,
                y - HANDLE_SIZE/2.0,
                HANDLE_SIZE - 1, HANDLE_SIZE - 1,
                (Color *)&COLOR_BLACK);
    }

    void Handle::addUpdate(IDoc *doc)
    {
        if (!doc) 
            return;

        // 应该调用
        // doc->addUpdatePixels();
        Point pos = m_data->pos;
        Rectangle r;
        r.setX(pos.x() - HANDLE_SIZE/2.0);
        r.setY(pos.y() - HANDLE_SIZE/2.0);
        r.setWidth(HANDLE_SIZE);
        r.setHeight(HANDLE_SIZE);

        doc->addUpdate(&r);
    }

    int Handle::isClicked(View *ddisp, Point *pos)
    {
        Rectangle rect = Rectangle(m_data->pos.x() - HANDLE_SIZE / 2.0,
                                   m_data->pos.y() - HANDLE_SIZE / 2.0,
                                   HANDLE_SIZE,
                                   HANDLE_SIZE);

        return rect.contains(*pos);
    }

}


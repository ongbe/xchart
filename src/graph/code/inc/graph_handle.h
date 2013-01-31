

#ifndef _GRAPH_HANDLE_H_
#define _GRAPH_HANDLE_H_

#include "graph_geometry.h"


namespace GraphLib
{

    class GraphRenderer;
    class View;
    class IDoc;

    /*! Some object resizing depends on the placement of the handle */
    typedef enum {
        HANDLE_RESIZE_NW, /*!< north/west or top/left */
        HANDLE_RESIZE_N,  /*!< north or top */
        HANDLE_RESIZE_NE, /*!< north/east or top/right */
        HANDLE_RESIZE_W,  /*!< west or left */
        HANDLE_RESIZE_E,  /*!< east or right */
        HANDLE_RESIZE_SW, /*!< south/west or bottom/left */
        HANDLE_RESIZE_S,  /*!< south or bottom */
        HANDLE_RESIZE_SE, /*!< south/east or bottom/right */
        HANDLE_MOVE_STARTPOINT, /*!< for lines: the beginning */
        HANDLE_MOVE_ENDPOINT,   /*!< for lines: the ending */

        /*! These handles can be used privately by objects: */
        HANDLE_CUSTOM1=200,
        HANDLE_CUSTOM2,
        HANDLE_CUSTOM3,
        HANDLE_CUSTOM4,
        HANDLE_CUSTOM5,
        HANDLE_CUSTOM6,
        HANDLE_CUSTOM7,
        HANDLE_CUSTOM8,
        HANDLE_CUSTOM9
    } HandleId;

    /*! HandleType is used for color coding the different handles */
    typedef enum {
        HANDLE_NON_MOVABLE,
        HANDLE_MAJOR_CONTROL,
        HANDLE_MINOR_CONTROL,

        NUM_HANDLE_TYPES /* Must be last */
    }  HandleType;


    /*! When an objects move_handle() function is called this is passed in */
    typedef enum {
        HANDLE_MOVE_USER,
        HANDLE_MOVE_USER_FINAL,
        //HANDLE_MOVE_CONNECTED,
        HANDLE_MOVE_CREATE,       /*!< the initial drag during object placement */
        HANDLE_MOVE_CREATE_FINAL  /*!< finish of initial drag */
    } HandleMoveReason;


    class Handle
    {
        public:
            explicit Handle();
            ~Handle();

        public:
            Point pos();
            void setPos(Point pos);
            HandleId id();
            void setId(HandleId id);
            HandleType type();
            void setType(HandleType type);

        public:
            void draw(GraphRenderer *);
            void addUpdate(IDoc *doc);
            int isClicked(View *ddisp, Point *pos);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif


#ifndef _GRAPH_TOOL_H_
#define _GRAPH_TOOL_H_

class QMouseEvent;
class QWidget;
class IView;

namespace GraphLib
{

    class Tool
    {
        public:
            enum ToolType
            {
                CREATE_OBJECT_TOOL,
                MAGNIFY_TOOL,
                MODIFY_TOOL,
                SCROLL_TOOL
            };

            struct ToolState
            {
                ToolType type;
                void *extraData;
                void *userData;
                QWidget *button;
                int invertPersistence;
            };
        public:
            virtual ~Tool();
            virtual int type();

        public:
            virtual void buttonPressFunc(QMouseEvent *);
            virtual void buttonHoldFunc(QMouseEvent *);
            virtual void doubleClickFunc(QMouseEvent *);
            virtual void buttonReleaseFunc(QMouseEvent *);
            virtual void motionFunc(QMouseEvent *, IView *);
    };
}

#endif

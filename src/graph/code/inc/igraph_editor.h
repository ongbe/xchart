
#ifndef _IGRAPH_EDIROT_H_
#define _IGRAPH_EDIROT_H_

#include "graph_geometry.h"

#include <QtGlobal>
//#include <QWidget>


namespace GraphLib
{
    /**
     * 将editor设计成对doc的特殊的交互编辑器
     * 可以访问IDoc中的内容，可以共享IView的image数据
     * */
    class IDoc;
    class IView;
    class IEditor 
    {
        public:
            virtual ~IEditor() {}

        public:
            virtual IDoc *doc() = 0;
            virtual IView *doc() = 0;
            //virtual QWidget *widget() = 0;
    };
}

#endif


#ifndef _GRAPH_GROUP_H_
#define _GRAPH_GROUP_H_

#include "graph_object.h"

namespace GraphLib
{
 
    class GraphGroup : public  Object
    {
        public:
            GraphGroup();
            ~GraphGroup();

        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}

#endif

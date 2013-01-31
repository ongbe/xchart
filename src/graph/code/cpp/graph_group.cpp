
#include "graph_group.h"

namespace GraphLib
{
    struct GraphGroup::PrivateData
    {
    };

    GraphGroup::GraphGroup() : Object()
    {
        m_data = new PrivateData;
    }

    GraphGroup::~GraphGroup()
    {
        delete m_data;
    }

}



GRAPH_LIB_INC = $$PWD/inc
GRAPH_LIB_CPP = $$PWD/cpp

INCLUDEPATH += \
            $${GRAPH_LIB_INC} \
            $${GRAPH_LIB_CPP} \

DEPENDPATH += \
            $${GRAPH_LIB_INC} \
            $${GRAPH_LIB_CPP} \

HEADERS += \
       $$PWD/inc/graph_image.h \
       $$PWD/inc/graph_box.h \
       $$PWD/inc/graph_ellipse.h \
       $$PWD/inc/graph_line.h \
       $$PWD/inc/graph_textobj.h \
       $$PWD/inc/graph_polygon.h \
       $$PWD/inc/graph_polyline.h \

SOURCES += \
       $$PWD/cpp/graph_ellipse.cpp \
       $$PWD/cpp/graph_image.cpp \
       $$PWD/cpp/graph_box.cpp \
       $$PWD/cpp/graph_line.cpp \
       $$PWD/cpp/graph_textobj.cpp \
       $$PWD/cpp/graph_polygon.cpp \
       $$PWD/cpp/graph_polyline.cpp \

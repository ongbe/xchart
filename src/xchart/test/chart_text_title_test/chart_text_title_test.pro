######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 14 10:11:15 2010
######################################################################

TEMPLATE = app
TARGET = a.out
QT += xml

DEPENDPATH += . \
        $$PWD/../../code/inc/ \
        $$PWD/../../code/cpp/ \

INCLUDEPATH += . \
        $$PWD/../../code/inc/ \


#GRAPH_LIB_PATH = $$PWD/../../../../../deployment/libs/graph_lib/unix/
#unix:LIBS += -L$${GRAPH_LIB_PATH} -lgraph_lib
include($$PWD/../../../graph/code/graph_lib.pri)
#include($$PWD/../../code/cross.pri)

HEADERS += \
        window.h \
        $$PWD/../../code/inc/chart_title.h \
        $$PWD/../../code/inc/chart_text_title.h \

# Input
SOURCES += main.cpp \
        window.cpp \
        $$PWD/../../code/cpp/chart_title.cpp \
        $$PWD/../../code/cpp/chart_text_title.cpp \


######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 14 10:11:15 2010
######################################################################

TEMPLATE = app
TARGET = a.out
CONFIG += warn_off

QT += xml

DEPENDPATH += . \
        $$PWD/../../code/inc/ \
        $$PWD/../../code/cpp/ \

INCLUDEPATH += . \
        $$PWD/../../code/inc/ \


#GRAPH_LIB_PATH = $$PWD/../../../../../deployment/libs/graph_lib/unix/
#unix:LIBS += -L$${GRAPH_LIB_PATH} -lgraph_lib
include($$PWD/../../../graph_lib/code/graph_lib.pri)
include($$PWD/../../code/chart.pri)

HEADERS += \
        window.h \

# Input
SOURCES += main.cpp \
        window.cpp \


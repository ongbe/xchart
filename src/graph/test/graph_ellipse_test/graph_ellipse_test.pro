
TEMPLATE = app
TARGET = a.out
QT += xml
QT += xml

DEPENDPATH += . \
        $$PWD/../../code/inc/ \
        $$PWD/../../code/cpp/ \

INCLUDEPATH += . \
        $$PWD/../../code/inc/ \

include($$PWD/../../code/graph_lib.pri)

HEADERS += window.h \

SOURCES += main.cpp \
           window.cpp \


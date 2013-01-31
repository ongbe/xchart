TEMPLATE = lib

CONFIG += qt staticlib 

QT += xml 

include($$PWD/../../../graph_lib/code/graph_lib.pri)
include($$PWD/../../code/chart.pri)

DESTDIR = $$PWD/../


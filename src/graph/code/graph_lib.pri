
GRAPH_LIB_INC = $$PWD/inc
GRAPH_LIB_CPP = $$PWD/cpp

INCLUDEPATH += \
            $${GRAPH_LIB_INC} \
            $${GRAPH_LIB_CPP} \

DEPENDPATH += \
            $${GRAPH_LIB_INC} \
            $${GRAPH_LIB_CPP} \

include($$PWD/graph_objects_standard.pri)
include($$PWD/graph_core1.pri)
include($$PWD/graph_tile.pri)
#include($$PWD/graph_agg.pri)

HEADERS += \
    $${GRAPH_LIB_INC}/graph_types.h \
    $${GRAPH_LIB_INC}/graph_font.h \
    $${GRAPH_LIB_INC}/graph_image.h \
    $${GRAPH_LIB_INC}/graph_color.h \
    $${GRAPH_LIB_INC}/graph_geometry.h \
    $${GRAPH_LIB_INC}/graph_renderer.h \
    $${GRAPH_LIB_INC}/i_canvas_path_methods.h \
    $${GRAPH_LIB_INC}/graph_qt_renderer.h \
    $${GRAPH_LIB_INC}/graph_path_renderer.h \
    $${GRAPH_LIB_INC}/igraph_object.h \
    $${GRAPH_LIB_INC}/graph_object.h \
    $${GRAPH_LIB_INC}/graph_parent.h \
    $${GRAPH_LIB_INC}/graph_group.h \
    $${GRAPH_LIB_INC}/graph_data.h \
    $${GRAPH_LIB_INC}/graph_layer.h \
    $${GRAPH_LIB_INC}/graph_view.h \
    $${GRAPH_LIB_INC}/graph_display.h \
    #$${GRAPH_LIB_INC}/graph_panel.h \
    $${GRAPH_LIB_INC}/igraph_view.h \
    #$${GRAPH_LIB_INC}/igraph_editor.h \
    $${GRAPH_LIB_INC}/igraph_doc.h \
    $${GRAPH_LIB_INC}/igraph_interactive_renderer.h \
    $${GRAPH_LIB_INC}/graph_qt_interactive_renderer.h \
    $${GRAPH_LIB_INC}/graph_path_interactive_renderer.h \
    $${GRAPH_LIB_INC}/graph_element.h \
    $${GRAPH_LIB_INC}/graph_handle.h \
    $${GRAPH_LIB_INC}/graph_object_change.h \
    $${GRAPH_LIB_INC}/graph_doc.h \
    $${GRAPH_LIB_INC}/graph_tool.h \
    $${GRAPH_LIB_INC}/graph_modify_tool.h \
    $${GRAPH_LIB_INC}/graph_text.h \
    $${GRAPH_LIB_INC}/graph_text_line.h \
    $${GRAPH_LIB_INC}/graph_poly_shape.h \
    $${GRAPH_LIB_INC}/graph_polyconn.h \
    $${GRAPH_LIB_INC}/graph_bounding_box.h \

SOURCES += \
    $${GRAPH_LIB_CPP}/graph_font.cpp \
    $${GRAPH_LIB_CPP}/graph_geometry.cpp \
    $${GRAPH_LIB_CPP}/graph_renderer.cpp \
    $${GRAPH_LIB_CPP}/graph_qt_renderer.cpp \
    $${GRAPH_LIB_CPP}/graph_path_renderer.cpp \
    $${GRAPH_LIB_CPP}/graph_object.cpp \
    $${GRAPH_LIB_CPP}/graph_parent.cpp \
    $${GRAPH_LIB_CPP}/graph_group.cpp \
    $${GRAPH_LIB_CPP}/graph_data.cpp \
    $${GRAPH_LIB_CPP}/graph_layer.cpp \
    $${GRAPH_LIB_CPP}/graph_view.cpp \
    $${GRAPH_LIB_CPP}/graph_display.cpp \
    #$${GRAPH_LIB_CPP}/graph_panel.cpp \
    $${GRAPH_LIB_CPP}/graph_qt_interactive_renderer.cpp \
    $${GRAPH_LIB_CPP}/graph_path_interactive_renderer.cpp \
    $${GRAPH_LIB_CPP}/graph_element.cpp \
    $${GRAPH_LIB_CPP}/graph_handle.cpp \
    $${GRAPH_LIB_CPP}/graph_object_change.cpp \
    $${GRAPH_LIB_CPP}/graph_doc.cpp \
    $${GRAPH_LIB_CPP}/graph_tool.cpp \
    $${GRAPH_LIB_CPP}/graph_modify_tool.cpp \
    $${GRAPH_LIB_CPP}/graph_text.cpp \
    $${GRAPH_LIB_CPP}/graph_text_line.cpp \
    $${GRAPH_LIB_CPP}/graph_poly_shape.cpp \
    $${GRAPH_LIB_CPP}/graph_polyconn.cpp \
    $${GRAPH_LIB_CPP}/graph_bounding_box.cpp \


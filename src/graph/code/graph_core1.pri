
GRAPH_CORE_INC = $$PWD/inc
GRAPH_CORE_CPP = $$PWD/cpp


INCLUDEPATH += \
            $${GRAPH_CORE_INC} \
            $${GRAPH_CORE_CPP} \

DEPENDPATH += \
            $${GRAPH_CORE_INC} \
            $${GRAPH_CORE_CPP} \

HEADERS += \
    $${GRAPH_CORE_INC}/graph_global.h \
    $${GRAPH_CORE_INC}/graph_valuelist.h \
    $${GRAPH_CORE_INC}/graph_math.h \
    $${GRAPH_CORE_INC}/graph_scale_div.h \
    $${GRAPH_CORE_INC}/graph_scale_engine.h \
    $${GRAPH_CORE_INC}/graph_double_interval.h \
    $${GRAPH_CORE_INC}/graph_array.h \
    $${GRAPH_CORE_INC}/graph_double_rect.h \
    $${GRAPH_CORE_INC}/graph_scale_map.h \
    $${GRAPH_CORE_INC}/graph_clipper.h \
    $${GRAPH_CORE_INC}/graph_spline.h \
    $${GRAPH_CORE_INC}/graph_curve_fitter.h \

#    $${GRAPH_CORE_INC}/graph_layout_metrics.h \
#    $${GRAPH_CORE_INC}/graph_color_map.h \
#    $${GRAPH_CORE_INC}/graph_spline.h \

SOURCES += \
    $${GRAPH_CORE_CPP}/graph_math.cpp \
    $${GRAPH_CORE_CPP}/graph_scale_div.cpp \
    $${GRAPH_CORE_CPP}/graph_scale_engine.cpp \
    $${GRAPH_CORE_CPP}/graph_double_interval.cpp \
    $${GRAPH_CORE_CPP}/graph_scale_map.cpp \
    $${GRAPH_CORE_CPP}/graph_clipper.cpp \
    $${GRAPH_CORE_CPP}/graph_spline.cpp \
    $${GRAPH_CORE_CPP}/graph_curve_fitter.cpp \ 

#    $${GRAPH_CORE_CPP}/graph_layout_metrics.cpp \
#    $${GRAPH_CORE_CPP}/graph_color_map.cpp \
#    $${GRAPH_CORE_CPP}/graph_spline.cpp \


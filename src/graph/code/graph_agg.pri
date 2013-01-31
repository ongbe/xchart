

INCLUDEPATH += \
               $$PWD/inc \

INCLUDEPATH +=  $$PWD/../../include \
                $$PWD/../../../../3rdparty/agg/agg-2.5/include \
                $$PWD/../../../../3rdparty/agg/agg-2.5/font_freetype \
                $$PWD/../../../../3rdparty/agg/agg-2.5/font_win32_tt \
                #$$PWD/../../../../3rdparty/agg/agg-2.5/gpc \
                /usr/include/freetype2/ \


DEPENDPATH += \
              $$PWD/inc \
              $$PWD/cpp \



# agg的原始文件 #
HEADERS += \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_basics.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_rendering_buffer.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_color_rgba.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_dda_line.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_renderer_base.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_renderer_primitives.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_pixfmt_rgb.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_pixfmt_rgba.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_rasterizer_scanline_aa.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_scanline_u.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_renderer_scanline.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_ellipse.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_transform.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_trans_affine.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_trans_single_path.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_contour.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_stroke.h \
    #$$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_gpc.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_clip_polygon.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_clip_polyline.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vcgen_bspline.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vcgen_contour.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vcgen_stroke.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_adaptor_vcgen.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_path_storage.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_font_cache_manager.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/font_freetype/agg_font_freetype.h \
#    $$PWD/../../../../3rdparty/agg/agg-2.5/font_win32_tt/agg_font_win32_tt.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_conv_curve.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_curves.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_bspline.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vpgen_segmentator.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vpgen_clip_polygon.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_vpgen_clip_polyline.h \
    $$PWD/../../../../3rdparty/agg/agg-2.5/include/agg_bounding_rect.h \
    #$$PWD/../../../../3rdparty/agg/agg-2.5/gpc/gpc.h \

# Input
SOURCES += \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_trans_single_path.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vcgen_bspline.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vcgen_contour.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vcgen_stroke.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vpgen_segmentator.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vpgen_clip_polygon.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_vpgen_clip_polyline.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_curves.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_bspline.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/font_freetype/agg_font_freetype.cpp \
    $$PWD/../../../../3rdparty/agg/agg-2.5/src/agg_trans_affine.cpp \
    #$$PWD/../../../../3rdparty/agg/agg-2.5/gpc/gpc.c \
#    $$PWD/../../../../3rdparty/agg/agg-2.5/font_win32_tt/agg_font_win32_tt.cpp \


#  扩展的文件 #
HEADERS += \
    $$PWD/inc/graph_agg_renderer.h \
    $$PWD/inc/graph_agg_interactive_renderer.h \
    $$PWD/inc/agg_interactive_polygon.h \
    $$PWD/inc/agg_path_attributes.h \

SOURCES += \
    $$PWD/cpp/graph_agg_renderer.cpp \
    $$PWD/cpp/graph_agg_interactive_renderer.cpp \
    $$PWD/cpp/agg_interactive_polygon.cpp \



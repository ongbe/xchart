#ifndef _PATH_H_ 
#define _PATH_H_


#include "agg_basics.h"
#include "agg_pixfmt_rgb.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"

namespace GraphLib 
{
    using namespace agg;
    //============================================================================
    // Basic path attributes
    struct path_attributes
    {
        unsigned     index;
        rgba8        fill_color;
        rgba8        stroke_color;
        bool         fill_flag;
        bool         stroke_flag;
        bool         even_odd_flag;
        line_join_e  line_join;
        line_cap_e   line_cap;
        double       miter_limit;
        double       stroke_width;
        trans_affine transform;

        // 自定义path的类型，区分不同的clip方式
        unsigned path_clip_type; // 0 polyline 1 polygon 2 text
        char type[32];
        agg::rect_d bbox;

        // Empty constructor
        path_attributes() :
            index(0),
            fill_color(rgba(0,0,0)),
            stroke_color(rgba(0,0,0)),
            fill_flag(true),
            stroke_flag(false),
            even_odd_flag(false),
            line_join(miter_join),
            line_cap(butt_cap),
            miter_limit(4.0),
            stroke_width(1.0),
            transform(),
            path_clip_type(0)
        {
        }

        // Copy constructor
        path_attributes(const path_attributes& attr) :
            index(attr.index),
            fill_color(attr.fill_color),
            stroke_color(attr.stroke_color),
            fill_flag(attr.fill_flag),
            stroke_flag(attr.stroke_flag),
            even_odd_flag(attr.even_odd_flag),
            line_join(attr.line_join),
            line_cap(attr.line_cap),
            miter_limit(attr.miter_limit),
            stroke_width(attr.stroke_width),
            transform(attr.transform),
            path_clip_type(attr.path_clip_type)
        {
        }

        // Copy constructor with new index value
        path_attributes(const path_attributes& attr, unsigned idx) :
            index(idx),
            fill_color(attr.fill_color),
            stroke_color(attr.stroke_color),
            fill_flag(attr.fill_flag),
            stroke_flag(attr.stroke_flag),
            even_odd_flag(attr.even_odd_flag),
            line_join(attr.line_join),
            line_cap(attr.line_cap),
            miter_limit(attr.miter_limit),
            stroke_width(attr.stroke_width),
            transform(attr.transform),
            path_clip_type(attr.path_clip_type)
        {
        }
    };

}

#endif

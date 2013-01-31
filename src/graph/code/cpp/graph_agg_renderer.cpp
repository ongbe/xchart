
#include "graph_agg_renderer.h"
#include "graph_font.h"
#include "agg_interactive_polygon.h"
#include "agg_path_attributes.h"

// tile相关的文件
#include "tile_types.h"
#include "tile_manager.h"
#include "tile.h"


// agg库相关的文件
#include "agg_basics.h"
#include "agg_math_stroke.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_ellipse.h"
#include "agg_conv_transform.h"
#include "agg_conv_segmentator.h"
#include "agg_conv_bspline.h"
#include "agg_conv_contour.h"
#include "agg_conv_stroke.h"
//#include "agg_conv_gpc.h"
#include "agg_conv_clip_polyline.h"
#include "agg_path_storage.h"
#include "agg_trans_affine.h"
#include "agg_trans_single_path.h"
#include "agg_bounding_rect.h"

#include "agg_pixfmt_rgba.h"

//#include "agg_font_win32_tt.h"
#include "agg_font_freetype.h"
#include "agg_font_cache_manager.h"
#include "agg_array.h"


typedef agg::renderer_base< agg::pixfmt_bgra32 > ren_base;
typedef agg::renderer_scanline_aa_solid< ren_base > renderer_solid;
typedef agg::scanline_u8 scanline_type;
typedef agg::font_engine_freetype_int16 font_engine_type;
typedef agg::font_cache_manager<font_engine_type> font_manager_type;

typedef agg::conv_curve<font_manager_type::path_adaptor_type> cc_pa_type;
typedef agg::conv_stroke<cc_pa_type> cs_cc_pa_type;
typedef agg::conv_transform<cs_cc_pa_type> ct_cs_cc_pa_type;


static font_manager_type *g_fman = NULL;

static font_manager_type *get_fman()
{
    if (g_fman == NULL) {
        static font_engine_type feng;
        static font_manager_type fman(feng);

        //agg::glyph_rendering gren = agg::glyph_ren_agg_gray8;
        agg::glyph_rendering gren = agg::glyph_ren_outline;
        feng.load_font("/home/zlk/workroom/trunk/qtreserve/"
                "src/lib_render/test/text_test/simsun.ttc",
                //if (feng.load_font("/home/zlk/workroom/trunk/qtreserve/src/lib_render/test/aa_demo_test/MSYH.TTF",
            0, gren);

        feng.height(36.0/2);
        feng.width(40.0/2);
        feng.hinting(true);
        feng.flip_y(true);

        g_fman = &fman;
    }

    return g_fman;
}


#include <QDebug>
#include <QPainter>
#include <QImage>
#include <QTransform>
#include <QTime>
#include <exception>


namespace GraphLib
{
    template <class Src> struct conv_poly_counter
    {
    };

    typedef agg::pod_bvector<path_attributes> attr_storage;

    static QRectF calculate_extent(QRectF r)
    {
        // 确保image的大小，从左上角的(0,0)位置位起点。
        return r.united(QRectF(0, 0, 1, 1));
        // 测试的范围
        //return QRectF(0, 0, 1000, 1000);
    }

    struct AggText {
        QString text;
        agg::rgba8 color;
        int alignment;
        Point start;
    };

    struct GraphAggRenderer::PrivateData
    {
        IGraphInteractiveRenderer *interactiveRenderer;

        // 存储绘图内容
        agg::path_storage ps;
        attr_storage as;
        attr_storage attr_stack;
        agg::trans_affine ts;
        std::vector<AggText> texts;

        TileManager *tiles; 
        unsigned char *redrawFlags;
        QRectF bbox;
        int width;
        int height;
        QRegion *clipRegion;

        GraphFont *font;
    };

    GraphAggRenderer::GraphAggRenderer() : GraphRenderer()
    {
        m_data = new PrivateData;
        m_data->interactiveRenderer = NULL;

        m_data->tiles = 0;
        m_data->bbox = QRectF(0, 0, 0, 0);
        m_data->redrawFlags = 0;
        m_data->clipRegion = NULL;
        m_data->width = 0;
        m_data->height = 0;

        m_data->font = new GraphFont;
    }
    GraphAggRenderer::~GraphAggRenderer()
    {
        delete m_data;
    }

    void GraphAggRenderer::beginRender()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        m_data->ps.remove_all();
        m_data->as.remove_all();
        m_data->bbox = QRectF();
        m_data->texts.clear();
    }
    void GraphAggRenderer::endRender()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        QRectF extent = calculate_extent(m_data->bbox);

        // 范围改变，重新分配tile 
        if (!m_data->tiles || m_data->tiles->width < extent.width() ||
                m_data->tiles->height < extent.height()) {
            if (m_data->tiles)
                delete m_data->tiles;
            m_data->tiles = new TileManager(extent.width(), 
                    extent.height(), 4);

            TileManager *tm = m_data->tiles;
            int ntiles = tm->ntile_rows * tm->ntile_cols;
            if (m_data->redrawFlags) {
                delete []m_data->redrawFlags;
            }
            m_data->redrawFlags = new unsigned char[ntiles];
            memset(m_data->redrawFlags, 1, ntiles);
        }

        if (m_data->clipRegion) {
            TileManager *tm = m_data->tiles;
            int ntiles = tm->ntile_rows * tm->ntile_cols;
            QRectF bbox = m_data->clipRegion->boundingRect();
            int x = bbox.x(); 
            int y = bbox.y(); 
            int w = bbox.width();
            int h = bbox.height();
            int i, j;
            for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))  {
                for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH))) {
                    int index = tm->get_tile_num(j, i);
                    if (-1 != index) {
                        m_data->redrawFlags[index] = 1;
                    }
                }
            }
        }
    }

     IGraphInteractiveRenderer *GraphAggRenderer::interactiveRenderer()
    {
        return m_data->interactiveRenderer;
    }
    void GraphAggRenderer::setInteractiveRenderer(IGraphInteractiveRenderer *r)
    {
        m_data->interactiveRenderer = r;
    }

    void GraphAggRenderer::drawLine(Point *start, Point *end, 
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(start->x(), start->y());
        ps.line_to(end->x(), end->y());
        ps.end_poly(agg::path_cmd_stop);

        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "drawLine");
        attr.path_clip_type = 100;
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::drawRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        //qDebug() << *lrCorner;
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(ulCorner->x(), ulCorner->y());
        ps.line_to(lrCorner->x(), ulCorner->y());
        ps.line_to(lrCorner->x(), lrCorner->y());
        ps.line_to(ulCorner->x(), lrCorner->y());
        //ps.line_to(ulCorner->x(), ulCorner->y());
        ps.end_poly(agg::path_flags_close);


        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        attr.path_clip_type = 100;
        //qDebug() << "path_clip_type "<< attr.path_clip_type;
        strcpy(attr.type, "drawRect");
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
        //qDebug() << idx << " " << *ulCorner << "--" << *lrCorner;
    }

    void GraphAggRenderer::fillRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(ulCorner->x(), ulCorner->y());
        ps.line_to(lrCorner->x(), ulCorner->y());
        ps.line_to(lrCorner->x(), lrCorner->y());
        ps.line_to(ulCorner->x(), lrCorner->y());
        //ps.line_to(ulCorner->x(), ulCorner->y());
        ps.end_poly(agg::path_flags_close);


        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "fillRect");
        attr.stroke_flag = false;
        attr.fill_flag = true;
        attr.fill_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }
 

    void GraphAggRenderer::drawEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::ellipse e1;
        e1.init(center->x(), center->y(),
                width, height, 200);

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();
        ps.concat_path(e1);

        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "drawEllipse");
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }



    void GraphAggRenderer::fillEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::ellipse e1;
        e1.init(center->x(), center->y(),
                width, height, 100);

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();
        ps.concat_path(e1);

        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "fillEllipse");
        attr.stroke_flag = false;
        attr.fill_flag = true;
        attr.fill_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::drawPolyline(Point *points, int pointCount,
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(points[0].x(), points[0].y());
        for (int i = 1; i < pointCount; ++i) {
            ps.line_to(points[i].x(), points[i].y());
        }
        ps.end_poly(agg::path_cmd_stop);

        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "drawPolyline");
        attr.path_clip_type = 100;
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::drawPolygon(Point *points, int pointCount,
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(points[0].x(), points[0].y());
        for (int i = 1; i < pointCount; ++i) {
            ps.line_to(points[i].x(), points[i].y());
        }
        //ps.move_to(points[0].x(), points[0].y());
        ps.end_poly(agg::path_flags_close);


        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "drawPolygon");
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::fillPolygon(Point *points, int pointCount,
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        ps.move_to(points[0].x(), points[0].y());
        for (int i = 1; i < pointCount; ++i) {
            ps.line_to(points[i].x(), points[i].y());
        }
        ps.end_poly(agg::path_flags_close);
        


        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "fillPolygon");
        attr.stroke_flag = false;
        attr.fill_flag = true;
        attr.fill_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::drawString(const QString &text, 
            Point *pos,
            int alignment, 
            Color *color)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        //return;

        if (text.isEmpty())
            return;

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();


        // 绘制文本
        font_manager_type *fman = get_fman();

        double x = pos->x();
        double y = pos->y();

        wchar_t *buffer = new wchar_t[text.size() + 1];
        text.toWCharArray(buffer);
        buffer[text.size()] = NULL;
        //qDebug() << text;
        const wchar_t *p = buffer;//L"this is a deom.";
        while (*p) {
            //qDebug() << QString(*p);

            const agg::glyph_cache* glyph = fman->glyph(*p);
            if (glyph) {
                //qDebug() << QString("good");
                fman->init_embedded_adaptors(glyph, x, y);
                ps.concat_path(fman->path_adaptor());

                x += glyph->advance_x;
                y += glyph->advance_y;
                //qDebug() << QString("%1,%2").arg(x).arg(y);
            } else {
                //qDebug() << QString("bad");
            }
            ++p;
        }
        delete []buffer;


        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));

        //保存idx值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "drawString");
        attr.path_clip_type = 100;
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.stroke_color = agg::rgba8(color->red(), 
                color->green(), color->blue(), color->alpha());
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }

    void GraphAggRenderer::copyToWindow(QPainter *d,
            int x, int y, int w, int h)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        if (NULL == d || m_data->tiles == 0) {
            return;
        }
        QTime time;
        time.start();

        GraphLib::TileManager *tm = m_data->tiles;

        QTransform t;
        t.translate(-1 * x, -1 * y);

        QPainter &painter = *d;
        painter.setTransform(t);

        int i, j;
        for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))  {
            for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH))) {
                GraphLib::Tile *t = tm->get_tile(j, i, true, false);
                if (t == NULL)
                    continue;
                Tile::release(t, false);

                if (1 == needRedraw(QRectF(j, i, 1, 1))) {
                    t = tm->get_tile(j, i, true, true);

                    int tile_x, tile_y;
                    tm->get_tile_coordinates(t, &tile_x, &tile_y);
                    QImage image(t->data, t->ewidth, t->eheight, 
                            QImage::Format_ARGB32_Premultiplied);

                    memset(t->data, 0, t->size);
                    agg::trans_affine mtx;
                    QRectF extent = calculate_extent(m_data->bbox);

                    mtx *= agg::trans_affine_translation(-1*extent.center().x(), -1*extent.center().y());
                    mtx *= agg::trans_affine_translation(tm->width/2, tm->height/2);
                    mtx *= agg::trans_affine_translation(-1 * tile_x, -1 * tile_y);
                    render(&image, QRectF(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT), &mtx);
                    Tile::release(t, true);

                    int index = tm->get_tile_num(tile_x, tile_y);
                    m_data->redrawFlags[index] = 0;
                }

                t = tm->get_tile(j, i, true, false);
                int tile_x, tile_y;
                tm->get_tile_coordinates(t, &tile_x, &tile_y);

                QImage image(t->data, t->ewidth, t->eheight, 
                        QImage::Format_ARGB32_Premultiplied);

                int w = t->ewidth;
                int h = t->eheight;

                QRectF targetRect(tile_x, tile_y, w, h);
                QRectF sourceRect(0, 0, w, h);

                painter.drawImage(targetRect, image, sourceRect);
                Tile::release(t, false);


#ifdef TILE_PROFILING
                // 显示tile
                painter.setPen(Qt::red);
                int index = tm->get_tile_num(tile_x, tile_y);
                //image.save(QString("%1.png").arg(index));
                painter.drawText(QRectF(tile_x, tile_y, w, h),
                        Qt::AlignCenter, QString("%1").arg(index));
                painter.drawRect(QRectF(tile_x, tile_y, w, h));
#endif
            } // j循环
        } // i循环
        //qDebug() << "copyToWindow Time elapsed: " << time.elapsed() << " ms.";
    }

    void GraphAggRenderer::render(QImage *image1, QRectF clip, 
            agg::trans_affine *t)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        QTime time;
        time.start();

        agg::trans_affine transform = *t;

        QImage &image = *image1;
        agg::rendering_buffer rbuf(image.bits(),
                image.width(), 
                image.height(), 
                image.width() * 4);

        agg::pixfmt_bgra32 pixf(rbuf);

        ren_base ren(pixf);
        renderer_solid r(ren);
        ren.clear(agg::rgba(1, 1, 1));

        scanline_type sl;
        agg::rasterizer_scanline_aa<> ras;

        //qDebug() << "clip box" << clip;
        //qDebug() << "image " << image.height();
        //qDebug() << "path count " << m_data->as.size();
        agg::path_storage &ps = m_data->ps;

        int dbg_hit = 0;
        for (unsigned i = 0; i < m_data->as.size(); 
                ++i) {

            QTime time;
            time.start();

            const path_attributes &attr = m_data->as[i];

            // rect 过滤, 只绘制在tile相关的path
            agg::rect_d pathBounds = attr.bbox;
            QRectF rect = QRectF(pathBounds.x1, pathBounds.y1,
                    pathBounds.x2 - pathBounds.x1,
                    pathBounds.y2 - pathBounds.y1);
            //qDebug() << "rect box:" << rect;
            if (!clip.intersects(rect)) {
                //qDebug() << "---empty----";
                continue;
            } else {
                ++dbg_hit;
            }

            if (attr.fill_flag) {
                ras.reset();

                agg::conv_contour<agg::path_storage> cc(ps);
                agg::conv_transform
                    < agg::conv_contour 
                    < agg::path_storage > > ct(cc, transform);
                ras.add_path(ct, attr.index);

                r.color(attr.fill_color);
                agg::render_scanlines(ras, sl, r);


            } else if (attr.stroke_flag) {
                ras.reset();

                if (attr.path_clip_type == 100) {

                    agg::conv_transform< 
                        agg::path_storage 
                        > ctps(ps, transform);
                    agg::conv_clip_polyline< 
                        agg::conv_transform<
                        agg::path_storage 
                        > > ccp(ctps);
                    // 只绘制tile内部的点和线。
                    ccp.clip_box(0.0, 0.0, 256.0, 256.0);

                    agg::conv_stroke<
                        agg::conv_clip_polyline< 
                        agg::conv_transform< 
                        agg::path_storage 
                        > > > cs11(ccp);
                    cs11.width(attr.stroke_width);
                    //cs11.width(0.5);
                    //qDebug() << "====" << attr.stroke_width;
                    //ras.add_path(ccp, attr.index);
                    //ras.add_path(st, attr.index);
                    ras.add_path(cs11, attr.index);
                    //ras.add_path(ct, attr.index);
                    //
                    //qDebug() << attr.type << " conv_clip_polyline";
                } else {
                    /*
                    agg::conv_stroke<agg::path_storage> pg(ps);
                    agg::conv_transform< agg::conv_stroke < agg::path_storage >
                        > ct(pg, transform);

                    agg::path_storage ps1;
                    ps1.move_to(clip.x(), clip.y());
                    ps1.line_to(clip.x() + clip.width(), clip.y());
                    ps1.line_to(clip.x() + clip.width() , clip.y() + clip.height());
                    ps1.line_to(clip.x(), clip.y() + clip.height());
                    ps1.close_polygon();

                    agg::conv_gpc< agg::path_storage, 
                        agg::path_storage > gpc(ps,ps1);

                    agg::conv_stroke< 
                        agg::conv_gpc< agg::path_storage, agg::path_storage> 
                        > csgpc(gpc);

                    agg::conv_transform< 
                        agg::conv_stroke< 
                        agg::conv_gpc< agg::path_storage, agg::path_storage> 
                        > > ctcsgpc(csgpc, transform);


                    //ras.add_path(ct, attr.index);
                    //ras.add_path(gpc, attr.index);
                    //ras.add_path(ps, attr.index);
                    //ras.add_path(pg, attr.index);
                    //ras.add_path(gpc, attr.index);
                    //ras.add_path(tt, attr.index);
                    ras.add_path(ctcsgpc, attr.index);

                        */
                    /*
                       if (strcmp(attr.type, "drawRect") == 0) {
                       ctcsgpc.rewind(attr.index);
                       unsigned cmd; 
                       double x, y;
                       while ((cmd = ctcsgpc.vertex(&x, &y))
                       != agg::path_cmd_stop) {
                       qDebug() << "gpc " << attr.index << "(" << x << "," << y << ")";
                       }
                       }
                       */
                    //qDebug() << attr.type << " conv_gpc";
                }

                r.color(attr.stroke_color);
                agg::render_scanlines(ras, sl, r);

            }

            if (strcmp(attr.type, "drawRect") == 0) {
                //qDebug() << attr.index << " TYPE " << attr.type << time.elapsed() << " ms.";
            }

            if (strcmp(attr.type, "drawString") == 0) {
                //qDebug() << attr.index << " TYPE " << attr.type << time.elapsed() << " ms.";
            }
        }

        //qDebug() << "hit count " << dbg_hit;
        //qDebug() << "tile render Time elapsed: " << time.elapsed() << " ms.";
    }
    void GraphAggRenderer::pop_attr()
    {
        if(m_data->attr_stack.size() == 0) {
            //throw std::exception("pop_attr : Attribute stack is empty");
            throw std::exception();//"pop_attr : Attribute stack is empty");
        }
        m_data->attr_stack.remove_last();
    }

    void GraphAggRenderer::push_attr()
    {
        m_data->attr_stack.add(m_data->attr_stack.size() ? 
                m_data->attr_stack[m_data->attr_stack.size() - 1] :
                path_attributes());
    }

    path_attributes& GraphAggRenderer::cur_attr()
    {
        if(m_data->attr_stack.size() == 0) {
            //throw exception("cur_attr : Attribute stack is empty");
        }
        return m_data->attr_stack[m_data->attr_stack.size() - 1];
    }

    int GraphAggRenderer::needRedraw(QRectF tileRect)
    {
        TileManager *tm = m_data->tiles;
        int index = tm->get_tile_num(tileRect.x(), tileRect.y());
        return m_data->redrawFlags[index];
    }

    GraphFont *GraphAggRenderer::font()
    {
        return m_data->font;
    }

    void GraphAggRenderer::setFont(GraphFont *)
    {
        //qDebug() << "WARING not implement"; 
    }
    void GraphAggRenderer::setClipRegion(QRegion *region)
    {
        //qDebug() << "setClipRegion";
        if (m_data->clipRegion)
            delete m_data->clipRegion;
        m_data->clipRegion = region; 
    }

    QRegion *GraphAggRenderer::clipRegion()
    {
        if (!m_data->clipRegion)
            m_data->clipRegion = new QRegion;
        return m_data->clipRegion;
    }


    int GraphAggRenderer::widthPixels()
    {
        return m_data->width;
    }
    int GraphAggRenderer::heightPixels()
    {
        return m_data->height;
    }
    void GraphAggRenderer::setWidthPixels(int w)
    {
        m_data->width = w;
    }
    void GraphAggRenderer::setHeightPixels(int h)
    {
        m_data->height = h;
    }

    void GraphAggRenderer::updateBbox()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        if (m_data->as.size() == 0)
            return;

        path_attributes &attr = m_data->as[m_data->as.size() -1];
        int idx = attr.index;
        agg::rect_d &pathBounds = attr.bbox;
        agg::bounding_rect_single(m_data->ps, idx,
                &pathBounds.x1, &pathBounds.y1,
                &pathBounds.x2, &pathBounds.y2);

        QRectF rect = QRectF(pathBounds.x1, pathBounds.y1,
                pathBounds.x2 - pathBounds.x1,
                pathBounds.y2 - pathBounds.y1);

        m_data->bbox = m_data->bbox.united(rect);
    }


    void GraphAggRenderer::beginPath()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        agg::path_storage &ps = m_data->ps;
        unsigned idx = ps.start_new_path();

        // 保存属性
        push_attr();
        m_data->as.add(path_attributes(cur_attr(), idx));
    }
    void GraphAggRenderer::endPath()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        if (m_data->as.size() == 0) {
            // ERROR: 调用方式出错
            return;
        }

        agg::path_storage &ps = m_data->ps;
        ps.end_poly(agg::path_cmd_stop);

        // 更新属性值
        path_attributes attr = cur_attr();
        strcpy(attr.type, "endPath");
        unsigned idx = m_data->as[m_data->as.size() -1].index;
        attr.path_clip_type = 100;
        attr.stroke_flag = true;
        attr.fill_flag = false;
        attr.index = idx;
        m_data->as[m_data->as.size() -1] = attr;
        pop_attr();

        updateBbox();
    }
    void GraphAggRenderer::closePath()
    {
        qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
    }
    void GraphAggRenderer::moveTo(double x, double y)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        agg::path_storage &ps = m_data->ps;
        ps.move_to(x, y);
    }
    void GraphAggRenderer::lineTo(double x, double y)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        agg::path_storage &ps = m_data->ps;
        ps.line_to(x, y);
    }


    void GraphAggRenderer::setLineWidth(float width)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        if(m_data->attr_stack.size() == 0) {
            return;
        }
        cur_attr().stroke_width = double(width);

    }
}

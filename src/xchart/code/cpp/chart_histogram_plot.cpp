
#include "chart_histogram_plot.h"

#include "graph_renderer.h"

#include <QTransform>
#include <QDebug>

namespace Cross
{
    struct ChartHistogramPlot::PrivateData
    {
        ChartNumberAxis *valueAxis;
        ChartCategoryAxis *categoryAxis;

        ChartHistogramDataset *dataset;
        double margin;
        double padding;

        double barsPadding;

        double valueAxisWidth;
        double valueAxisHeight;

        double categoryAxisWidth;
        double categoryAxisHeight;

        double valueAxisLower;
        double valueAxisUpper;

        VALUE_SHOW_STYLE valueShowStyle;

        RectangleEdge rectangleEdge;

        bool valueAxisIsVisible;
        bool categoryAxisIsVisible;

        TickUnit tickUnit;

        bool gridIsVisible;

        QColor backgroundColor;
        bool backgroundIsVisible;

        QColor barsAreaBackgroundColor;
        bool barsAreaBackgroundIsVisible;

        QColor barColor;

        QColor barOutlineColor;
        double barOutlineWidth;
        bool barOutlineIsVisible;

        bool outlineIsVisible;
        bool barsAreaOutlineIsVisible;

        QColor outlineColor;
        double outlineWidth;

        QColor gridLineColor;
        double gridLineWidth;
    };

    ChartHistogramPlot::ChartHistogramPlot(ChartHistogramDataset *dataset) :
        ChartPlot(dataset)
    {
        m_data = new PrivateData;
        m_data->valueAxis = new ChartNumberAxis();
        m_data->categoryAxis = new ChartCategoryAxis();

        m_data->dataset = dataset;
        m_data->margin = 10;
        m_data->padding = 3;

        m_data->barsPadding = 3;

        m_data->valueAxisWidth = 30;
        m_data->valueAxisHeight = 25;

        m_data->categoryAxisWidth = 30;
        m_data->categoryAxisHeight = 25;

        m_data->valueAxisLower = 0;
        m_data->valueAxisUpper = 100;

        m_data->valueShowStyle = ChartHistogramPlot::VALUE_SHOW_STYLE_TOP;
        m_data->rectangleEdge = Cross::LEFT;

        m_data->valueAxisIsVisible = true;
        m_data->categoryAxisIsVisible = true;

        m_data->tickUnit.size = 10;
        m_data->tickUnit.minorTickCount = 3;

        m_data->gridIsVisible = true;

        m_data->backgroundColor = Qt::white;
        m_data->backgroundIsVisible = true;

        m_data->barsAreaBackgroundColor = Qt::white;//QColor(192, 192, 192);
        m_data->barsAreaBackgroundIsVisible = true;

        m_data->barColor = QColor(252, 86, 88);

        m_data->barOutlineColor = Qt::black;
        m_data->barOutlineWidth = 0.1;
        m_data->barOutlineIsVisible = true;

        m_data->outlineIsVisible = true;
        m_data->barsAreaOutlineIsVisible = true;

        m_data->outlineColor = Qt::gray;
        m_data->outlineWidth = 1.0;

        m_data->gridLineColor = Qt::darkGray;//Qt::white;
        m_data->gridLineWidth = 0.3;
    }

    ChartHistogramPlot::~ChartHistogramPlot()
    {
        delete m_data->valueAxis;
        delete m_data->categoryAxis;
        delete m_data;
    }

    double ChartHistogramPlot::margin()
    {
        return m_data->margin;
    }

    void ChartHistogramPlot::setMargin(double margin)
    {
        m_data->margin = margin;
    }

    double ChartHistogramPlot::padding()
    {
        return m_data->padding;
    }

    void ChartHistogramPlot::setPadding(double padding)
    {
        m_data->padding = padding;
    }

    double ChartHistogramPlot::barsPadding()
    {
        return m_data->barsPadding;
    }

    void ChartHistogramPlot::setBarsPadding(double barsPadding)
    {
        m_data->barsPadding = barsPadding;
    }

    ChartHistogramPlot::VALUE_SHOW_STYLE ChartHistogramPlot::valueShowStyle()
    {
        return m_data->valueShowStyle;
    }

    void ChartHistogramPlot::setValueShowStyle(ChartHistogramPlot::VALUE_SHOW_STYLE valueShowStyle)
    {
        m_data->valueShowStyle = valueShowStyle;
    }

    ChartHistogramDataset *ChartHistogramPlot::dataset()
    {
        return m_data->dataset;
    }

    void ChartHistogramPlot::setDataset(ChartHistogramDataset *dataset)
    {
        m_data->dataset = dataset;
    }

    bool ChartHistogramPlot::valueAxisIsVisible()
    {
        return m_data->valueAxisIsVisible;
    }

    void ChartHistogramPlot::setValueAxisVisible(bool visible)
    {
        m_data->valueAxisIsVisible = visible;
    }

    bool ChartHistogramPlot::categoryAxisIsVisible()
    {
        return m_data->categoryAxisIsVisible;
    }

    void ChartHistogramPlot::setCategoryAxisVisible(bool visible)
    {
        m_data->categoryAxisIsVisible = visible;
    }

    bool ChartHistogramPlot::gridIsVisible()
    {
        return m_data->gridIsVisible;
    }

    void ChartHistogramPlot::setGridVisible(bool visible)
    {
        m_data->gridIsVisible = visible;
    }

    QColor ChartHistogramPlot::backgroundColor()
    {
        return m_data->backgroundColor;
    }

    void ChartHistogramPlot::setBackgroundColor(QColor color)
    {
        m_data->backgroundColor = color;
    }

    bool ChartHistogramPlot::backgroundIsVisible()
    {
        return m_data->backgroundIsVisible;
    }

    void ChartHistogramPlot::setBackgroundVisible(bool visible)
    {
        m_data->backgroundIsVisible = visible;
    }

    bool ChartHistogramPlot::barsAreaBackgroundIsVisible()
    {
        return m_data->barsAreaBackgroundIsVisible;
    }

    void ChartHistogramPlot::setBarsAreaBackgroundVisible(bool visible)
    {
        m_data->barsAreaBackgroundIsVisible = visible;
    }


    QColor ChartHistogramPlot::barsAreaBackgroundColor()
    {
        return m_data->barsAreaBackgroundColor;
    }

    void ChartHistogramPlot::setBarsAreaBackgroundColor(QColor color)
    {
        m_data->barsAreaBackgroundColor = color;
    }

    QColor ChartHistogramPlot::barColor()
    {
        return m_data->barColor;
    }

    void ChartHistogramPlot::setBarColor(QColor color)
    {
        m_data->barColor = color;
    }

    double ChartHistogramPlot::valueAxisLower()
    {
        return m_data->valueAxisLower;
    }

    bool ChartHistogramPlot::barOutlineIsVisible()
    {
        return m_data->barOutlineIsVisible;
    }

    void ChartHistogramPlot::setBarOutlineVisible(bool visible)
    {
        m_data->barOutlineIsVisible = visible;
    }

    void ChartHistogramPlot::setValueAxisLower(double value)
    {
        m_data->valueAxisLower = value;
    }


    bool ChartHistogramPlot::outlineIsVisible()
    {
        return m_data->outlineIsVisible;
    }

    void ChartHistogramPlot::setOutlineVisible(bool visible)
    {
        m_data->outlineIsVisible = visible;
    }


    bool ChartHistogramPlot::barsAreaOutlineIsVisible()
    {
        return m_data->barsAreaOutlineIsVisible;
    }

    void ChartHistogramPlot::setBarsAreaOutlineVisible(bool visible)
    {
        m_data->barsAreaOutlineIsVisible = visible;
    }

    double ChartHistogramPlot::valueAxisUpper()
    {
        return m_data->valueAxisUpper;
    }

    void ChartHistogramPlot::setValueAxisUpper(double value)
    {
        m_data->valueAxisUpper = value;
    }

    Cross::RectangleEdge ChartHistogramPlot::rectangleEdge()
    {
        return m_data->rectangleEdge;
    }

    void ChartHistogramPlot::setRectangleEdge(Cross::RectangleEdge rectangleEdge)
    {
        m_data->rectangleEdge = rectangleEdge;
    }

    ChartNumberAxis* ChartHistogramPlot::valueAxis() const
    {
        return m_data->valueAxis;
    }

    ChartCategoryAxis* ChartHistogramPlot::categoryAxis() const
    {
        return m_data->categoryAxis;
    }

    void ChartHistogramPlot::draw(GraphLib::GraphRenderer *renderer,
            QRectF plotArea,
            QPointF anchor)
    {
        drawBackground(renderer, plotArea);

        if (m_data->dataset != NULL && m_data->dataset->keys().size() > 0) {
            switch (m_data->rectangleEdge) {
                case TOP :
                    drawRectangleEdgeTop(renderer, plotArea);
                    break;
                case LEFT :
                    drawRectangleEdgeLeft(renderer, plotArea);
                    break;
                default :
                    break;
            }
        }

        drawOutline(renderer, plotArea);
    }

    void ChartHistogramPlot::drawRectangleEdgeTop(GraphLib::GraphRenderer *renderer,
                                            QRectF plotArea)
    {
        double margin = m_data->margin;
        double padding = m_data->padding;

        bool valueAxisIsVisible = m_data->valueAxisIsVisible;
        bool categoryAxisIsVisible = m_data->categoryAxisIsVisible;

        QRectF valueAxisArea;
        QRectF categoryAxisArea;

        if (valueAxisIsVisible && categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin + m_data->categoryAxisWidth + padding,
                                   plotArea.top() + margin,
                                   plotArea.width() - margin * 2 - padding - m_data->categoryAxisWidth,
                                   m_data->valueAxisHeight);

            categoryAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin + m_data->valueAxisHeight + padding,
                                      m_data->categoryAxisWidth,
                                      plotArea.height() - margin * 2 - m_data->valueAxisHeight - padding);

        } else if (!valueAxisIsVisible && categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin + m_data->categoryAxisWidth + padding,
                                   plotArea.top() + margin,
                                   plotArea.width() - margin * 2 - padding - m_data->categoryAxisWidth,
                                   0);

            categoryAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin,
                                      m_data->categoryAxisWidth,
                                      plotArea.height() - margin * 2);

        } else if (valueAxisIsVisible && !categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin,
                                   plotArea.top() + margin,
                                   plotArea.width() - margin * 2,
                                   m_data->valueAxisHeight);

            categoryAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin + m_data->valueAxisHeight + padding,
                                      0,
                                      plotArea.height() - margin * 2 - m_data->valueAxisHeight - padding);

        } else { //(!valueAxisIsVisible && !categoryAxisIsVisible)

            valueAxisArea = QRectF(plotArea.left() + margin,
                                   plotArea.top() + margin,
                                   plotArea.width() - margin * 2,
                                   0);

            categoryAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin,
                                      0,
                                      plotArea.height() - margin * 2);

        }

        QRectF barsArea = QRectF(valueAxisArea.left(),
                                 categoryAxisArea.top(),
                                 valueAxisArea.width(),
                                 categoryAxisArea.height());

        QRectF gridArea = barsArea;

        m_data->valueAxis->setTickUnit(m_data->tickUnit);
        m_data->valueAxis->setRange(m_data->valueAxisLower, m_data->valueAxisUpper);

        drawBarsAreaBackground(renderer, barsArea);

        if (m_data->gridIsVisible)
            drawGrid(renderer, gridArea, Cross::TOP);

        QList<CategoryTick> categoryTicks;

        double y = barsArea.top();
        for (int i = 0; i < m_data->dataset->keys().size(); i++) {
            QString key = m_data->dataset->keys().at(i);
            double value = m_data->dataset->value(key);

            QString text = QString("%1").arg(value);

            double x = m_data->valueAxis->valueToChart2D(value, barsArea, Cross::TOP);

            double height = (barsArea.height() - m_data->barsPadding) / m_data->dataset->keys().size() - m_data->barsPadding;

            QRectF barRect = QRectF(barsArea.left(),
                                    y + m_data->barsPadding,
                                    x - barsArea.left(),
                                    height);

            drawBar(renderer, barRect, text, m_data->valueShowStyle);

            CategoryTick categoryTick;
            categoryTick.label = key;
            categoryTick.value = barRect.center().y();
            categoryTicks << categoryTick;

            y += height;
        }

        m_data->categoryAxis->setCategoryTicks(categoryTicks);

        drawBarsAreaOutline(renderer, barsArea);

        if (valueAxisIsVisible)
            drawValueAxis(renderer, plotArea, barsArea, valueAxisArea, TOP);
        if (categoryAxisIsVisible)
            drawCategoryAxis(renderer, plotArea, barsArea, categoryAxisArea, LEFT);
    }

    void ChartHistogramPlot::drawRectangleEdgeLeft(GraphLib::GraphRenderer *renderer,
                                                    QRectF plotArea)
    {
        double margin = m_data->margin;
        double padding = m_data->padding;

        bool valueAxisIsVisible = m_data->valueAxisIsVisible;
        bool categoryAxisIsVisible = m_data->categoryAxisIsVisible;

        QRectF valueAxisArea;
        QRectF categoryAxisArea;

        if (valueAxisIsVisible && categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin,
                                      m_data->valueAxisWidth,
                                      plotArea.height() - margin * 2 - m_data->categoryAxisHeight - padding);

            categoryAxisArea = QRectF(valueAxisArea.right() + padding,
                                      valueAxisArea.bottom() + padding,
                                      plotArea.right() - valueAxisArea.right() - margin - padding,
                                      m_data->categoryAxisHeight);

        } else if (!valueAxisIsVisible && categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin,
                                      0,
                                      plotArea.height() - margin * 2 - m_data->categoryAxisHeight - padding);

            categoryAxisArea = QRectF(valueAxisArea.right(),
                                      valueAxisArea.bottom() + padding,
                                      plotArea.right() - valueAxisArea.right() - margin - padding,
                                      m_data->categoryAxisHeight);

        } else if (valueAxisIsVisible && !categoryAxisIsVisible) {

            valueAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.top() + margin,
                                      m_data->valueAxisWidth,
                                      plotArea.height() - margin * 2);

            categoryAxisArea = QRectF(valueAxisArea.right() + padding,
                                      valueAxisArea.bottom(),
                                      plotArea.right() - valueAxisArea.right() - margin - padding,
                                      0);

        } else { //(!valueAxisIsVisible && !categoryAxisIsVisible)

            valueAxisArea = QRectF(plotArea.left() + margin,
                                   plotArea.top() + margin,
                                   0,
                                   plotArea.height() - margin * 2);

            categoryAxisArea = QRectF(plotArea.left() + margin,
                                      plotArea.bottom() - margin,
                                      plotArea.width() - margin * 2,
                                      0);

        }


        QRectF barsArea = QRectF(categoryAxisArea.left(),
                                 valueAxisArea.top(),
                                 categoryAxisArea.width(),
                                 valueAxisArea.height());

        QRectF gridArea = barsArea;

        m_data->valueAxis->setTickUnit(m_data->tickUnit);
        m_data->valueAxis->setRange(m_data->valueAxisLower, m_data->valueAxisUpper);

        drawBarsAreaBackground(renderer, barsArea);

        if (m_data->gridIsVisible)
            drawGrid(renderer, gridArea, Cross::LEFT);

        QList<CategoryTick> xTicks;

        double x = barsArea.left();
        for (int i = 0; i < m_data->dataset->keys().size(); i++) {
            QString key = m_data->dataset->keys().at(i);
            double value = m_data->dataset->value(key);

            QString text = QString("%1").arg(value);

            GraphLib::Point point1 = QPointF(x, barsArea.top());
            GraphLib::Point point2 = QPointF(x, barsArea.bottom());


            double y = m_data->valueAxis->valueToChart2D(value, barsArea, Cross::LEFT);

            double width = (barsArea.width() - m_data->barsPadding) / m_data->dataset->keys().size() - m_data->barsPadding;

            QRectF barRect = QRectF(x + m_data->barsPadding,
                                    y,
                                    width,
                                    barsArea.bottom() - y);
            drawBar(renderer, barRect, text, m_data->valueShowStyle);

            CategoryTick xTick;
            xTick.label = key;
            xTick.value = barRect.center().x();
            xTicks << xTick;

            x = barRect.right();
        }

        m_data->categoryAxis->setCategoryTicks(xTicks);

        drawBarsAreaOutline(renderer, barsArea);

        if (valueAxisIsVisible)
            drawValueAxis(renderer, plotArea, barsArea, valueAxisArea, LEFT);

        if (categoryAxisIsVisible)
            drawCategoryAxis(renderer, plotArea, barsArea, categoryAxisArea, BOTTOM);
    }

    void ChartHistogramPlot::drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->backgroundIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->backgroundColor;


        renderer->fillRect(&point1, &point2, &color);

    }

    void ChartHistogramPlot::drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->outlineIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->outlineColor;

        renderer->setLineWidth(m_data->outlineWidth);
        renderer->drawRect(&point1, &point2, &color);
        renderer->setLineWidth(1);

    }

    void ChartHistogramPlot::drawGrid(GraphLib::GraphRenderer *renderer, QRectF rect, RectangleEdge edge)
    {
        QList<double> vs;

        QList<NumberTick> ticks = m_data->valueAxis->ticks(renderer, rect, edge);

        foreach (NumberTick numberTick, ticks)
            if (numberTick.type == MAJOR)
                vs << m_data->valueAxis->valueToChart2D(numberTick.value, rect, edge);

        GraphLib::Color color = m_data->gridLineColor;



        switch (edge) {

        case TOP: {
                foreach (double x, vs) {
                    GraphLib::Point point1 = QPointF(x, rect.top());
                    GraphLib::Point point2 = QPointF(x, rect.bottom());

                    renderer->setLineStyle(Qt::DashLine);
                    renderer->setLineWidth(m_data->gridLineWidth);
                    renderer->drawLine(&point1, &point2, &color);
                    renderer->setLineStyle(Qt::SolidLine);
                }
            }
            break;

        case LEFT: {
                foreach (double y, vs) {
                    GraphLib::Point point1 = QPointF(rect.left(), y);
                    GraphLib::Point point2 = QPointF(rect.right(), y);

                    renderer->setLineStyle(Qt::DashLine);
                    renderer->setLineWidth(m_data->gridLineWidth);
                    renderer->drawLine(&point1, &point2, &color);
                    renderer->setLineStyle(Qt::SolidLine);
                }
            }
            break;

        default:
            break;
        }

    }

    void ChartHistogramPlot::drawBarsAreaBackground(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->barsAreaBackgroundIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->barsAreaBackgroundColor;


        renderer->fillRect(&point1, &point2, &color);

    }

    void ChartHistogramPlot::drawBarsAreaOutline(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->barsAreaOutlineIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = QColor(96, 96, 96);


        renderer->drawRect(&point1, &point2, &color);

    }

    void ChartHistogramPlot::drawBar(GraphLib::GraphRenderer *renderer,
                                     QRectF rect,
                                     QString text,
                                     VALUE_SHOW_STYLE valueShowStyle)
    {

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color outlineColor = Qt::black;
        GraphLib::Color fillColor = m_data->barColor;

        GraphLib::Color fontColor = Qt::black;



        renderer->fillRect(&point1, &point2, &fillColor);

        if (m_data->barOutlineIsVisible) {
            GraphLib::Color barOutlineColor = m_data->barOutlineColor;
            renderer->setLineWidth(m_data->barOutlineWidth);
            renderer->drawRect(&point1, &point2, &barOutlineColor);
            renderer->setLineWidth(1);
        }

        GraphLib::GraphFont *font = renderer->font();
        double fontHeight = font->height();
        double fontWidth = font->stringWidth(text);

        switch (valueShowStyle) {
            case ChartHistogramPlot::VALUE_SHOW_STYLE_HIDE : {

                }
                break;

            case ChartHistogramPlot::VALUE_SHOW_STYLE_TOP : {
                    GraphLib::Point pointText = QPointF( rect.center().x() - fontWidth / 2,
                                                         rect.top() - 3);
                    renderer->drawString(text, &pointText, 0, &fontColor);
                }
                break;

            case ChartHistogramPlot::VALUE_SHOW_STYLE_CENTER: {
                    GraphLib::Point pointText = QPointF( rect.center().x() - fontWidth / 2,
                                                         rect.center().y() + fontHeight / 2);
                    renderer->drawString(text, &pointText, 0, &fontColor);
                }
                break;

            case ChartHistogramPlot::VALUE_SHOW_STYLE_LEFT : {
                    GraphLib::Point pointText = QPointF( rect.right() + 3,
                                                         rect.center().y() + fontHeight / 2);
                    renderer->drawString(text, &pointText, 0, &fontColor);
                }
                break;

            default:
                break;
        }

    }

    void ChartHistogramPlot::drawValueAxis(GraphLib::GraphRenderer *renderer,
                                           QRectF plotRect,
                                           QRectF dataRect,
                                           QRectF axisRect,
                                           RectangleEdge edge)
    {
        double cursor = 0;

        switch (edge) {
        case LEFT :
            cursor = axisRect.right();
            break;
        case TOP :
            cursor = axisRect.bottom();
            break;
        default :
            break;
        }


        m_data->valueAxis->draw(renderer,
                                cursor,
                                plotRect,
                                dataRect,
                                edge);

    }

    void ChartHistogramPlot::drawCategoryAxis(GraphLib::GraphRenderer *renderer,
                                              QRectF plotRect,
                                              QRectF dataRect,
                                              QRectF axisRect,
                                              RectangleEdge edge)
    {
        double cursor = 0;

        switch (edge) {
        case LEFT :
            cursor = axisRect.right();
            break;
        case BOTTOM :
            cursor = axisRect.top();
            break;
        default :
            break;
        }


        m_data->categoryAxis->draw(renderer,
                                   cursor,
                                   plotRect,
                                   dataRect,
                                   edge);

    }

    QColor ChartHistogramPlot::outlineColor()
    {
        return m_data->outlineColor;
    }

    void ChartHistogramPlot::setOutlineColor(QColor color)
    {
        m_data->outlineColor = color;
    }

    double ChartHistogramPlot::outlineWidth()
    {
        return m_data->outlineWidth;
    }

    void ChartHistogramPlot::setOutlineWidth(double width)
    {
        m_data->outlineWidth = width;
    }


    QColor ChartHistogramPlot::barOutlineColor()
    {
        return m_data->barOutlineColor;
    }

    void ChartHistogramPlot::setBarOutlineColor(QColor color)
    {
        m_data->barOutlineColor = color;
    }

    double ChartHistogramPlot::barOutlineWidth()
    {
        return m_data->barOutlineWidth;
    }

    void ChartHistogramPlot::setBarOutlineWidth(double width)
    {
        m_data->barOutlineWidth = width;
    }

    QColor ChartHistogramPlot::gridLineColor()
    {
        return m_data->gridLineColor;
    }

    void ChartHistogramPlot::setGridLineColor(QColor color)
    {
        m_data->gridLineColor = color;
    }

    double ChartHistogramPlot::gridLineWidth()
    {
        return m_data->gridLineWidth;
    }

    void ChartHistogramPlot::setGridLineWidth(double width)
    {
        m_data->gridLineWidth = width;
    }
}

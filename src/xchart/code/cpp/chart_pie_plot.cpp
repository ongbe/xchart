
#include "chart_pie_plot.h"
#include "graph_renderer.h"
#include "chart_label.h"

#include <QTransform>
#include <QDebug>

namespace Cross
{
    struct ChartPiePlot::PrivateData
    {
        ChartPieDataset *dataset;
        double margin;

        bool anchorIsVisible;
        double anchorSize;
        QColor anchorColor;

        ChartPiePlot::ShowStyle showStyle;
        ChartPiePlot::LabelStyle labelStyle;

        QColor backgroundColor;
        bool backgroundIsVisible;

        QColor outlineColor;
        double outlineWidth;
        bool outlineIsVisible;
    };

    ChartPiePlot::ChartPiePlot(ChartPieDataset *dataset) :
        ChartPlot(dataset)
    {
        m_data = new PrivateData;
        m_data->dataset = dataset;
        m_data->margin = 20;

        m_data->anchorIsVisible = false;
        m_data->anchorSize = 4;
        m_data->anchorColor = Qt::darkGray;

        m_data->showStyle = KEYWITHVALUE;
        m_data->labelStyle = DOGLEG;

        m_data->backgroundColor = Qt::white;
        m_data->outlineColor = Qt::gray;
        m_data->outlineWidth = 1.0;

        m_data->outlineIsVisible = true;
        m_data->backgroundIsVisible = true;
    }

    ChartPiePlot::~ChartPiePlot()
    {
        delete m_data;
    }

    ChartPiePlot::ShowStyle ChartPiePlot::showStyle()
    {
        return m_data->showStyle;
    }

    void ChartPiePlot::setShowStyle(ChartPiePlot::ShowStyle showStyle)
    {
        m_data->showStyle = showStyle;
    }

    ChartPiePlot::LabelStyle ChartPiePlot::labelStyle()
    {
        return m_data->labelStyle;
    }

    void ChartPiePlot::setLabelStyle(ChartPiePlot::LabelStyle style)
    {
        m_data->labelStyle = style;
    }

    ChartPieDataset *ChartPiePlot::dataset()
    {
        return m_data->dataset;
    }

    void ChartPiePlot::setDataset(ChartPieDataset *dataset)
    {
        m_data->dataset = dataset;
    }


    void ChartPiePlot::draw(GraphLib::GraphRenderer *renderer,
            QRectF plotArea,
            QPointF anchor)
    {
        drawBackground(renderer, plotArea);
        drawOutline(renderer, plotArea);

        if (m_data->dataset != NULL && m_data->dataset->keys().size() > 0) {
            drawPie(renderer, plotArea, anchor);
        } else {
            drawNoDataMessage();
        }
    }
    void ChartPiePlot::drawPie(GraphLib::GraphRenderer *renderer,
            QRectF plotArea,
            QPointF anchor)
    {
        if (m_data->dataset == NULL 
                && m_data->dataset->keys().size() == 0)
            return;

        double margin = m_data->margin;
        double left = plotArea.left();
        double top = plotArea.top();
        double right = plotArea.right();
        double width = plotArea.width();
        double height = plotArea.height();

        double widthPie = (width > height) ? height : width;
        if (widthPie > margin * 2)
            widthPie -= margin *2;

        double heightPie = (width > height) ? height : width;
        if (heightPie > margin * 2)
            heightPie -= margin *2;

        if (m_data->labelStyle != HIDE) {
            if (widthPie > width / 2) {
                widthPie = width / 2;
                heightPie = widthPie;
            }
        }

        double leftPie = left + (width - widthPie) / 2;
        double topPie = top + (height - heightPie) / 2;
        double rightPie = leftPie + widthPie;
        double centerXPie = leftPie + widthPie / 2;
        double centerYPie = topPie + heightPie / 2;

        QRectF rectPie = QRectF(QPointF(leftPie, topPie),
                QSize(widthPie, heightPie));
        QRectF rectLeftLabels = QRectF(QPointF(left, top),
                QSize(leftPie - left, height));
        QRectF rectRightLabels = QRectF(QPointF(rightPie, top),
                QSize(right - rightPie, height));

        QMap<QString, QString> mapContent;
        QMap<QString, QColor> mapColor;
        QMap<QString, double> mapStartAngle;
        QMap<QString, double> mapAngle;
        QMap<QString, QPointF> mapAnchorPoint;
        QMap<QString, QPointF> mapKneePoint;

        QMap<QString, QPointF> mapSimpleLabelPoint;

        double totalValue = m_data->dataset->totalValue();
        double latestAngle = 0.0;
        double latestAngleAnchor = 0.0;
        for (int i = 0; i < m_data->dataset->keys().count(); i++)  {
            QString key = m_data->dataset->keys().at(i);
            double value = m_data->dataset->value(key);
            double percentage = value / totalValue * 100;
            QColor color = pieColor(i);

            QString content;
            switch (m_data->showStyle) {
                case KEY:
                    content = QString("%1").arg(key);
                    break;
                case VALUE:
                    content = QString("%1").arg(value);
                    break;
                case PERCENTAGE:
                    content = QString("%1%").arg(percentage);
                    break;
                case KEYWITHVALUE:
                    content = QString("%1(%2)").arg(key).arg(value);
                    break;
                case KEYWITHRERCENTAGE:
                    content = QString("%1(%2%)").arg(key).arg(percentage);
                    break;
                default :
                    break;
            }

            double startAngle = latestAngle;
            double endAngle = startAngle + value / totalValue * 360.0;
            double angle = endAngle - startAngle;


            QTransform transform;
            transform.translate(rectPie.center().x(), rectPie.center().y());
            transform.rotate(latestAngleAnchor - angle / 2);
            latestAngleAnchor -= angle;
            QPointF pointAnchor = transform.map(QPointF(rectPie.width() / 2 - 5, 0));
            QPointF pointKnee = transform.map(QPointF(rectPie.width() / 2 + 5, 0));

            QPointF pointSimpleLabel = transform.map(QPointF(rectPie.width() / 4, 0));

            mapContent.insert(key, content);
            mapColor.insert(key, color);
            mapStartAngle.insert(key, startAngle);
            mapAngle.insert(key, angle);
            mapAnchorPoint.insert(key, pointAnchor);
            mapSimpleLabelPoint.insert(key, pointSimpleLabel);
            mapKneePoint.insert(key, pointKnee);

            latestAngle += angle;
        }

        QMap<QString, QPointF> mapLabelPoint;
        QMap<QString, QVector<GraphLib::Point> > mapDoglegPoints;

        foreach (QString key, mapAnchorPoint.keys()) {
            QPointF pointAnchor = mapAnchorPoint.value(key);
            QPointF pointKnee = mapKneePoint.value(key);
            QString content = mapContent.value(key);
            QSizeF labelSize = ChartLabel::size(renderer, content);

            double legX;
            double legY;

            double labelX;
            double labelY;

            if (pointKnee.x() < centerXPie) {
                labelX = rectLeftLabels.right() - labelSize.width() - 10; //左
                legX = labelX + labelSize.width();
            } else {
                labelX = rectRightLabels.left() + 10; //右
                legX = labelX;
            }

            labelY = pointKnee.y() + labelSize.height() / 2;

            legY = pointKnee.y();

            QPointF pointLabel(labelX, labelY);
            mapLabelPoint.insert(key, pointLabel);

            QVector<GraphLib::Point> points;
            points.push_back(pointAnchor);
            points.push_back(pointKnee);
            points.push_back(QPointF(legX, legY));
            mapDoglegPoints.insert(key, points);
        }


        for (int i = 0; i < m_data->dataset->keys().count(); i++)  {
            QString key = m_data->dataset->keys().at(i);

            double startAngle = mapStartAngle.value(key);
            double angle = mapAngle.value(key);
            QColor color = mapColor.value(key);

            drawItem(renderer,
                    rectPie,
                    startAngle,
                    angle,
                    color);
        }

        for (int i = 0; i < m_data->dataset->keys().count(); i++)  {
            QString key = m_data->dataset->keys().at(i);

            QString content = mapContent.value(key);

            QPointF pointSimpleLabel = mapSimpleLabelPoint.value(key);
            QPointF pointLabel = mapLabelPoint.value(key);
            QVector<GraphLib::Point> pointsDogleg = mapDoglegPoints.value(key);

            switch (m_data->labelStyle) {
                case DOGLEG:
                    drawLabel(renderer, pointLabel, content, pointsDogleg);
                    break;
                case SIMPLE:
                    drawSimpleLabel(renderer, pointSimpleLabel, content);
                    break;
                case HIDE:
                    break;
                default :
                    break;
            }
        }

    }

    QColor ChartPiePlot::backgroundColor()
    {
        return m_data->backgroundColor;
    }

    void ChartPiePlot::setBackgroundColor(QColor color)
    {
        m_data->backgroundColor = color;
    }

    void ChartPiePlot::drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->backgroundIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->backgroundColor;


        renderer->fillRect(&point1, &point2, &color);
    }

    QColor ChartPiePlot::outlineColor()
    {
        return m_data->outlineColor;
    }

    void ChartPiePlot::setOutlineColor(QColor color)
    {
        m_data->outlineColor = color;
    }

    double ChartPiePlot::outlineWidth()
    {
        return m_data->outlineWidth;
    }

    void ChartPiePlot::setOutlineWidth(double width)
    {
        m_data->outlineWidth = width;
    }

    bool ChartPiePlot::backgroundIsVisible()
    {
        return m_data->backgroundIsVisible;
    }

    void ChartPiePlot::setBackgroundVisible(bool visible)
    {
        m_data->backgroundIsVisible = visible;
    }

    bool ChartPiePlot::outlineIsVisible()
    {
        return m_data->outlineIsVisible;
    }

    void ChartPiePlot::setOutlineVisible(bool visible)
    {
        m_data->outlineIsVisible = visible;
    }

    double ChartPiePlot::margin()
    {
        return m_data->margin;
    }

    void ChartPiePlot::setMargin(double margin)
    {
        m_data->margin = margin;
    }

    void ChartPiePlot::drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect)
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

    void ChartPiePlot::drawItem(GraphLib::GraphRenderer *renderer,
            QRectF rect,
            double startAngle,
            double spanAngle,
            QColor color)
    {
        GraphLib::Point point = rect.topLeft();
        GraphLib::Color fillColor = color;
        GraphLib::Color outlineColor = Qt::darkGray;


        renderer->fillPie(&point,
                rect.width(),
                rect.height(),
                startAngle,
                spanAngle,
                &fillColor);

        renderer->setLineWidth(0.5);
        renderer->drawPie(&point,
                rect.width(),
                rect.height(),
                startAngle,
                spanAngle,
                &outlineColor);

    }

    void ChartPiePlot::drawLabel(GraphLib::GraphRenderer *renderer,
            QPointF point,
            QString content,
            QVector<GraphLib::Point> pointsDogleg)
    {
        //draw dog leg
        GraphLib::Color dogLegcolor = Qt::black;
        renderer->setLineWidth(0.5);
        renderer->drawPolyline(pointsDogleg.data(), pointsDogleg.size(), &dogLegcolor);
        renderer->setLineWidth(1);


        if (m_data->anchorIsVisible) {
            GraphLib::Color anchorColor = Qt::black;
            renderer->setLineWidth(0.5);
            renderer->drawPolyline(pointsDogleg.data(), pointsDogleg.size(), &anchorColor);
            renderer->setLineWidth(1);
        }

        ChartLabel label(content);
        label.draw(renderer, point);
    }

    void ChartPiePlot::drawSimpleLabel(GraphLib::GraphRenderer *renderer, 
            QPointF point, 
            QString content)
    {
        GraphLib::GraphFont *font = renderer->font();
        double fontHeight = font->height();
        double fontWidth = font->stringWidth(content);

        double x = point.x() - fontWidth / 2 + 3;
        double y = point.y() + fontHeight / 2;

        QPointF p = QPointF(x, y);

        GraphLib::Color color = Qt::black;

        renderer->drawString(content, &p, 0, &color);
    }

    void ChartPiePlot::drawNoDataMessage()
    {
    }

    QColor ChartPiePlot::pieColor(int index)
    {

        /** A very dark red color. */
#define VERY_DARK_RED QColor(0x80, 0x00, 0x00)

        /** A dark red color. */
#define DARK_RED QColor(0xc0, 0x00, 0x00)

        /** A light red color. */
#define LIGHT_RED QColor(0xFF, 0x40, 0x40)

        /** A very light red color. */
#define VERY_LIGHT_RED QColor(0xFF, 0x80, 0x80)

        /** A very dark yellow color. */
#define VERY_DARK_YELLOW QColor(0x80, 0x80, 0x00)

        /** A dark yellow color. */
#define DARK_YELLOW QColor(0xC0, 0xC0, 0x00)

        /** A light yellow color. */
#define LIGHT_YELLOW QColor(0xFF, 0xFF, 0x40)

        /** A very light yellow color. */
#define VERY_LIGHT_YELLOW QColor(0xFF, 0xFF, 0x80)

        /** A very dark green color. */
#define VERY_DARK_GREEN QColor(0x00, 0x80, 0x00)

        /** A dark green color. */
#define DARK_GREEN QColor(0x00, 0xC0, 0x00)

        /** A light green color. */
#define LIGHT_GREEN QColor(0x40, 0xFF, 0x40)

        /** A very light green color. */
#define VERY_LIGHT_GREEN QColor(0x80, 0xFF, 0x80)

        /** A very dark cyan color. */
#define VERY_DARK_CYAN QColor(0x00, 0x80, 0x80)

        /** A dark cyan color. */
#define DARK_CYAN QColor(0x00, 0xC0, 0xC0)

        /** A light cyan color. */
#define LIGHT_CYAN QColor(0x40, 0xFF, 0xFF)

        /** Aa very light cyan color. */
#define VERY_LIGHT_CYAN QColor(0x80, 0xFF, 0xFF)

        /** A very dark blue color. */
#define VERY_DARK_BLUE QColor(0x00, 0x00, 0x80)

        /** A dark blue color. */
#define DARK_BLUE QColor(0x00, 0x00, 0xC0)

        /** A light blue color. */
#define LIGHT_BLUE QColor(0x40, 0x40, 0xFF)

        /** A very light blue color. */
#define VERY_LIGHT_BLUE QColor(0x80, 0x80, 0xFF)

        /** A very dark magenta/purple color. */
#define VERY_DARK_MAGENTA QColor(0x80, 0x00, 0x80)

        /** A dark magenta color. */
#define DARK_MAGENTA QColor(0xC0, 0x00, 0xC0)

        /** A light magenta color. */
#define LIGHT_MAGENTA QColor(0xFF, 0x40, 0xFF)

        /** A very light magenta color. */
#define VERY_LIGHT_MAGENTA QColor(0xFF, 0x80, 0xFF)


        QList<QColor> list;

        list << QColor(0xFF, 0x55, 0x55);
        list << QColor(0x55, 0x55, 0xFF);
        list << QColor(0x55, 0xFF, 0x55);
        list << QColor(0xFF, 0xFF, 0x55);
        list << QColor(0xFF, 0x55, 0xFF);
        list << QColor(0x55, 0xFF, 0xFF);
        list << Qt::gray;
        list << DARK_RED;
        list << DARK_BLUE;
        list << DARK_GREEN;
        list << DARK_YELLOW;
        list << DARK_MAGENTA;
        list << DARK_CYAN;
        list << Qt::darkGray;
        list << LIGHT_RED;
        list << LIGHT_BLUE;
        list << LIGHT_GREEN;
        list << LIGHT_YELLOW;
        list << LIGHT_MAGENTA;
        list << LIGHT_CYAN;
        list << Qt::lightGray;
        list << VERY_DARK_RED;
        list << VERY_DARK_BLUE;
        list << VERY_DARK_GREEN;
        list << VERY_DARK_YELLOW;
        list << VERY_DARK_MAGENTA;
        list << VERY_DARK_CYAN;
        list << VERY_LIGHT_RED;
        list << VERY_LIGHT_BLUE;
        list << VERY_LIGHT_GREEN;
        list << VERY_LIGHT_YELLOW;
        list << VERY_LIGHT_MAGENTA;
        list << VERY_LIGHT_CYAN;

        return list.at(index % list.count());
    }

}


#include "chart_xy_dataset.h"

#include <QMap>

namespace Cross
{
    struct ChartXYDataset::PrivateData
    {
        QMap<QString, QVector<QPointF> > curves;
    };

    ChartXYDataset::ChartXYDataset() :
        ChartDataset()
    {
        m_data = new PrivateData;
    }

    ChartXYDataset::~ChartXYDataset()
    {
        delete m_data;
    }

    int ChartXYDataset::insertValue(const QString &key, const QPointF &v)
    {
        QVector<QPointF> curve = m_data->curves.value(key);
        curve.push_back(v);
        m_data->curves.insert(key, curve);
    }

    int ChartXYDataset::insertValues(const QString &key, 
            const QVector<QPointF> &val)
    {
        m_data->curves.insert(key, val);
    }

    QVector<QPointF> ChartXYDataset::values(const QString &key)
    {
        return m_data->curves.value(key);
    }

    QPointF ChartXYDataset::value(const QString &key, int index)
    {
        QPointF point(0,0);
        QVector<QPointF> curve = m_data->curves.value(key);
        if (index >= 0 && index < curve.size())
            return curve[index];
    }

    QStringList ChartXYDataset::keys() const
    {
        return m_data->curves.keys();
    }

    QRectF ChartXYDataset::rangeBox(const QString &key)
    {
        QVector<QPointF> ps = m_data->curves.value(key);

        double minX = 0;
        double maxX = 10;
        double minY = 0;
        double maxY = 10;

        if (ps.count() > 0) {

            minX = ps.first().x();
            maxX = ps.first().x();
            minY = ps.first().y();
            maxY = ps.first().y();

            foreach (QPointF p, ps) {

                double x = p.x();
                double y = p.y();

                if (minX > x)
                    minX = x;

                if (maxX < x)
                    maxX = x;

                if (minY > y)
                    minY = y;

                if (maxY < y)
                    maxY = y;
            }
        }

        return QRectF(minX, minY, maxX - minX, maxY - minY);
    }
}

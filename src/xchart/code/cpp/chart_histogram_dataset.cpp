
#include "chart_histogram_dataset.h"

#include <QMap>

namespace Cross
{
    struct ChartHistogramDataset::PrivateData
    {
        QVector<QString> keys;
        QMap<QString, double> map;
    };

    ChartHistogramDataset::ChartHistogramDataset() :
        ChartDataset()
    {
        m_data = new PrivateData;
    }

    ChartHistogramDataset::~ChartHistogramDataset()
    {
        delete m_data;
    }

    int ChartHistogramDataset::insertValue(QString key, double value)
    {
        m_data->keys.push_back(key);
        m_data->map.insert(key, value);
        return 0;
    }

    QVector<QString> ChartHistogramDataset::keys()
    {
        return m_data->keys;
    }

    double ChartHistogramDataset::value(QString key)
    {
        return m_data->map.value(key);
    }
}

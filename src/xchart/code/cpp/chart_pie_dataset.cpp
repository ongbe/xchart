
#include "chart_pie_dataset.h"

#include <QMap>

namespace Cross
{
    struct ChartPieDataset::PrivateData
    {
        QMap<QString, double> map;
    };

    ChartPieDataset::ChartPieDataset() :
        ChartDataset()
    {
        m_data = new PrivateData;
    }

    ChartPieDataset::~ChartPieDataset()
    {
        delete m_data;
    }

    int ChartPieDataset::insertValue(QString key, double value)
    {
        if (value <= 0)
            return -1;

        m_data->map.insert(key, value);

        return 0;
    }

    QList<QString> ChartPieDataset::keys()
    {
        return m_data->map.keys();
    }

    double ChartPieDataset::value(QString key)
    {
        return m_data->map.value(key);
    }

    double ChartPieDataset::totalValue()
    {
        double total = 0;

        foreach (QString key, m_data->map.keys())
            total += value(key);

        return total;
    }
}

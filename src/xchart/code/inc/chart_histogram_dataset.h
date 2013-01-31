
#ifndef _CHART_HISTOGRAM_DATASET_H_
#define _CHART_HISTOGRAM_DATASET_H_

#include "chart_dataset.h"

#include <QString>
#include <QVector>

namespace Cross
{
    /**
     * 直方图
     *
     * */
    class ChartHistogramDataset : public ChartDataset
    {
        public:
            ChartHistogramDataset();
            virtual ~ChartHistogramDataset();

        public:
            int insertValue(QString key, double value);

            QVector<QString> keys();
            double value(QString key);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

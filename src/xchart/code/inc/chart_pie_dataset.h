
#ifndef _CHART_PIE_DATASET_H_
#define _CHART_PIE_DATASET_H_

#include "chart_dataset.h"

#include <QString>
#include <QList>

namespace Cross
{
    /**
     * 饼图
     *
     * */
    class ChartPieDataset : public ChartDataset
    {
        public:
            ChartPieDataset();
            virtual ~ChartPieDataset();

        public:
            int insertValue(QString key, double value);

            QList<QString> keys();
            double value(QString key);

            // 有效值的总和。用来计算百分比
            double totalValue();

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

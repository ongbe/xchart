
#ifndef _CHART_XY_DATASET_H_
#define _CHART_XY_DATASET_H_

#include "chart_dataset.h"

#include <QString>
#include <QList>
#include <QPointF>
#include <QRectF>
#include <QVector>
#include <QStringList>

namespace Cross
{
    class ChartXYDataset : public ChartDataset
    {
        public:
            ChartXYDataset();
            virtual ~ChartXYDataset();

        public:
            int insertValue(const QString &key, const QPointF &);
            int insertValues(const QString &key, const QVector<QPointF> &);

            QVector<QPointF> values(const QString &key);
            QPointF value(const QString &key, int index);

            QRectF rangeBox(const QString &key);

            QStringList keys() const;

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif

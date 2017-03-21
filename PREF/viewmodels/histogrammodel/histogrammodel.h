#ifndef HISTOGRAMMODEL_H
#define HISTOGRAMMODEL_H

#include <QAbstractItemModel>
#include <qhistogram/qhistogram.h>
#include <chart/histogramchart.h>
#include "qhexedit/qhexeditdata.h"

using namespace PrefLib;
using namespace PrefLib::Chart;

class HistogramModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit HistogramModel(const HistogramChart& histogramchart, QHexEditData* hexeditdata, QObject *parent = 0);
        void updateStats();

    private:
        static void initNonAsciiChars();

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        static QMap<uchar, QString> _nonasciichars;
        const HistogramChart& _histogramchart;
        QHexEditData* _hexeditdata;
        QFont _monospacefont;

};

#endif // HISTOGRAMMODEL_H

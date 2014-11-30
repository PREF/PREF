#ifndef HISTOGRAMMODEL_H
#define HISTOGRAMMODEL_H

#include <QAbstractItemModel>
#include <qhexedit/qhexeditdata.h>
#include <qhistogram/qhistogram.h>

class HistogramModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit HistogramModel(QHexEditData *hexeditdata, QObject *parent = 0);
        void setOccurrenceList(const QList<qint64> occlist);

    private:
        static void initNonAsciiChars();
        void findMaxOccurence();

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
        QFont _monospacefont;
        QHexEditData* _hexeditdata;
        qint64 _maxoccidx;
        QList<qint64> _occurenceslist;
};

#endif // HISTOGRAMMODEL_H

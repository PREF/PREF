#ifndef STRINGSMODEL_H
#define STRINGSMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

class StringsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        typedef QList<qint64> OffsetList;
        typedef QPair<qint64, qint64> StringRange;
        typedef QHash<qint64, StringRange> StringMap;
        enum SearchDirection { Up, Down };

    public:
        explicit StringsModel(QHexEditData* hexeditdata, QObject *parent = 0);
        void setData(const StringsModel::OffsetList& offsetlist, const StringsModel::StringMap& strings);
        QModelIndex indexOf(const QString& searchstring, StringsModel::SearchDirection direction, const QModelIndex& startindex = QModelIndex());
        qint64 offset(int i) const;
        StringsModel::StringRange range(int i) const;
        QString string(int i) const;

    private:
        QModelIndex searchUp(const QString& searchstring, const QModelIndex& startindex);
        QModelIndex searchDown(const QString& searchstring, const QModelIndex& startindex);
    
    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    signals:
        void filterChanged();

    private:
        OffsetList _offsetlist;
        StringMap _strings;
        QHexEditDataReader* _reader;
        QFont _monospacefont;
};

#endif // STRINGSMODEL_H

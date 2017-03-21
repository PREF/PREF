#ifndef STRINGSMODEL_H
#define STRINGSMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <support/byteelaborator.h>
#include "qhexedit/qhexeditdatareader.h"

using namespace PrefLib::Support;

class StringsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum SearchDirection { Up, Down };

    public:
        explicit StringsModel(QHexEditData *hexeditdata, QObject *parent = 0);
        void setData(const ByteElaborator::StringList &strings);
        QModelIndex indexOf(const QString& searchstring, StringsModel::SearchDirection direction, const QModelIndex& startindex = QModelIndex());
        const ByteElaborator::StringRange& range(size_t i) const;
        uint64_t offset(size_t i) const;
        QString string(size_t i) const;

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
        ByteElaborator::StringList _strings;
        QHexEditDataReader* _reader;
        QFont _monospacefont;
};

#endif // STRINGSMODEL_H

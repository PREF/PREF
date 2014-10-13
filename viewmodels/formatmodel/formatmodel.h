#ifndef FORMATMODEL_H
#define FORMATMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"
#include "viewmodels/fielddatamodel/fielddatamodel.h"
#include "prefsdk/format/formattree.h"

using namespace PrefSDK;

class FormatModel : public FieldDataModel
{
    Q_OBJECT

    public:
        explicit FormatModel(QHexEditData* hexeditdata, QObject *parent = 0);
        QHexEditData* data();
        void setFormatTree(FormatTree *formattree);

    private slots:
        void updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual void fetchMore(const QModelIndex& parent);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &child) const;
        virtual bool canFetchMore(const QModelIndex &parent) const;
        virtual bool hasChildren(const QModelIndex &parent) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QFont _monospacefont;
        QImage _icostruct;
        QImage _icofield;
        QImage _icobitfield;
        QHexEditData* _hexeditdata;
        FormatTree* _formattree;
};

#endif // FORMATMODEL_H

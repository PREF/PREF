#ifndef TEMPLATEMODEL_H
#define TEMPLATEMODEL_H

#include <qhexedit/qhexedit.h>
#include "../platform/loadeddata.h"
#include "basicmodel.h"

class TemplateModel : public BasicItemModel
{
    Q_OBJECT

    public:
        explicit TemplateModel(QHexEdit *hexedit, QObject *parent = 0);
        ~TemplateModel();

    public:
        void execute(const QString& btfile);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
        virtual QModelIndex parent(const QModelIndex &child) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual int columnCount(const QModelIndex &) const;
        virtual int rowCount(const QModelIndex &parent) const;

    private:
        QHexEdit* _hexedit;
        LoadedData* _loadeddata;
        BTEntryList _template;
};

#endif // TEMPLATEMODEL_H

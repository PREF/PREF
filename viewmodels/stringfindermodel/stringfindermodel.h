#ifndef STRINGFINDERMODEL_H
#define STRINGFINDERMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "datatypes/indexkeymap.h"

class StringFinderModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit StringFinderModel(QObject *parent = 0);

    public slots:
        void clearItems();
        void addItem(quint64 offset, QString string);
        void filterString(QString s);
        int visibleStringsCount();
        bool filtered();
        qint64 offset(int i);
        QString string(int i);
    
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
        IndexKeyMap<qint64, QString> _offsetstringmap;
        IndexKeyMap<qint64, QString> _visiblestrings;
        bool _filtered;
};

#endif // STRINGFINDERMODEL_H

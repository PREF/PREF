#ifndef FILEFILTERMODEL_H
#define FILEFILTERMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class FileFilterModel : public QAbstractItemModel
{
    Q_OBJECT

    private:

    public:
        explicit FileFilterModel(QObject *parent = 0);
        QFileInfo file(int i);
        QFileInfo smallestFile();
        int smallestFileIndex();

    public slots:
        void setDirectory(QString s);
        void updateFilter(QString s);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QDir _dir;
        QString _filter;
        QFileInfoList _files;
        QImage _icofile;
};

#endif // FILEFILTERMODEL_H

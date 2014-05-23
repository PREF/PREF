#ifndef VISUALMAPMODEL_H
#define VISUALMAPMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "views/hexview/visualmapwidget/binarymap/binarymap.h"

class VisualMapModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit VisualMapModel(QObject *parent = 0);

    public slots:
        void setStartOffset(qint64 offset);
        void setCurrentOffset(qint64 offset);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    signals:
        void displayModeChanged(BinaryMap::DisplayMode displaymode);
        void currentOffsetChanged(qint64 offset);
        void widthChanged(qint64 width);

    private:
        QHash<BinaryMap::DisplayMode, QString> _modenames;
        QFont _monospacefont;
        BinaryMap::DisplayMode _mode;
        qint64 _startoffset;
        qint64 _currentoffset;
        qint64 _width;
};

#endif // VISUALMAPMODEL_H

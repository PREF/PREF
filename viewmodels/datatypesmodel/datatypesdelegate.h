#ifndef DATATYPESDELEGATE_H
#define DATATYPESDELEGATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "views/hexview/visualmapwidget/binarymap/binarymap.h"
#include "spinboxbaseselector/qnumberspinbox.h"

class DataTypesDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit DataTypesDelegate(QHexEditData *hexeditdata, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const;
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
        virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;

    private:
        QHexEditData* _hexeditdata;
};

#endif // DATATYPESDELEGATE_H

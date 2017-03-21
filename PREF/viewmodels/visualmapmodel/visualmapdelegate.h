#ifndef VISUALMAPDELEGATE_H
#define VISUALMAPDELEGATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "spinboxbaseselector/qnumberspinbox.h"
#include "views/hexview/visualmapwidget/binarymap/binarymap.h"

class VisualMapDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit VisualMapDelegate(QHexEditData* hexeditdata, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const;
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
        virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;

    private:
        QHexEditData* _hexeditdata;
};

#endif // VISUALMAPDELEGATE_H

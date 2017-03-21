#ifndef VISUALMAPWIDGET_H
#define VISUALMAPWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "views/hexview/workertab.h"
#include "viewmodels/visualmapmodel/visualmapdelegate.h"
#include "viewmodels/visualmapmodel/visualmapmodel.h"

namespace Ui {
class VisualMapWidget;
}

class VisualMapWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit VisualMapWidget(QWidget *parent = 0);
        void map(QHexEdit* hexedit);
        ~VisualMapWidget();

    private slots:
        void updateOffset(int);

    signals:
        void gotoTriggered(qint64);

    private:
        Ui::VisualMapWidget *ui;
        VisualMapModel* _visualmapmodel;
        VisualMapDelegate* _visualmapdelegate;
        QHexEdit* _hexedit;
};

#endif // VISUALMAPWIDGET_H

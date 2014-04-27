#ifndef PREFSDK_TABLEWIDGET_H
#define PREFSDK_TABLEWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace PrefSDK
{
    namespace Ui {
        class TableWidget;
    }

    class TableWidget : public QDialog
    {
        Q_OBJECT

        public:
            explicit TableWidget(QWidget *parent = 0);
            QTableWidget *table();
            ~TableWidget();

        private:
            Ui::TableWidget *ui;
    };
}
#endif // PREFSDK_TABLEWIDGET_H

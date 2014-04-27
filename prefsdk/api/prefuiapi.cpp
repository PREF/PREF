#include "prefuiapi.h"

namespace PrefSDK
{
    namespace API
    {
        TableWidget *PrefUI_createTableWidget(const char *title)
        {
            TableWidget* tablewidget = new TableWidget();
            tablewidget->setWindowTitle(QString::fromUtf8(title));
            return tablewidget;
        }

        void TableWidget_setColumnCount(TableWidget *__this, int count)
        {
            __this->table()->setColumnCount(count);
        }

        void TableWidget_setRowCount(TableWidget *__this, int count)
        {
            __this->table()->setRowCount(count);
        }

        void TableWidget_setHeaderItem(TableWidget *__this, int column, const char *text)
        {
            __this->table()->setHorizontalHeaderItem(column, new QTableWidgetItem(text));
            __this->table()->resizeColumnsToContents();
            __this->table()->resizeRowsToContents();
        }

        void TableWidget_setItem(TableWidget *__this, int row, int column, const char *text)
        {
            __this->table()->setItem(row, column, new QTableWidgetItem(text));
            __this->table()->resizeColumnsToContents();
            __this->table()->resizeRowsToContents();
        }

        void PrefUI_show(QDialog *__this)
        {
            __this->show();
        }

        void PrefUI_showModal(QDialog *__this)
        {
            __this->exec();
        }
    }
}

#include "prefuiapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Widget_resize(QWidget *__this, int w, int h)
        {
            __this->resize(w, h);
        }

        QLayout *Layout_createHorizontal()
        {
            return new QHBoxLayout();
        }

        QLayout *Layout_createVertical()
        {
            return new QVBoxLayout();
        }

        void BoxLayout_addLayout(QBoxLayout *__this, QLayout *layout, bool stretch)
        {
            __this->addLayout(layout, stretch);
        }

        void Layout_addWidget(QLayout *__this, QWidget *widget)
        {
            __this->addWidget(widget);
        }

        void Layout_setMargins(QLayout *__this, int left, int top, int right, int bottom)
        {
            __this->setContentsMargins(left, top, right, bottom);
        }

        QDialog *Dialog_create(const char *title)
        {
            QDialog* dialog = new QDialog();
            dialog->setWindowTitle(QString::fromUtf8(title));
            return dialog;
        }

        void Dialog_setLayout(QDialog *__this, QLayout *layout)
        {
            __this->setLayout(layout);
        }

        void Dialog_show(QDialog *__this)
        {
            __this->show();
        }

        void Dialog_exec(QDialog *__this)
        {
            __this->exec();
        }

        QTableWidget *TableWidget_create()
        {
            return new QTableWidget();
        }

        void TableWidget_setColumnCount(QTableWidget *__this, int count)
        {
            __this->setColumnCount(count);
        }

        void TableWidget_setRowCount(QTableWidget *__this, int count)
        {
            __this->setRowCount(count);
        }

        void TableWidget_setHeaderItem(QTableWidget *__this, int column, const char *text)
        {
            __this->setHorizontalHeaderItem(column, new QTableWidgetItem(text));
            __this->resizeColumnsToContents();
            __this->resizeRowsToContents();
        }

        void TableWidget_setItem(QTableWidget *__this, int row, int column, const char *text)
        {
            __this->setItem(row, column, new QTableWidgetItem(text));
            __this->resizeColumnsToContents();
            __this->resizeRowsToContents();
        }
    }
}

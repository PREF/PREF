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

        QTableView *TableWidget_create(int rows, int columns)
        {
            QTableView* tableview = new QTableView();
            tableview->setModel(new TableModel(rows, columns));
            return tableview;
        }

        void TableWidget_setHeaderItem(QTableView *__this, int column, const char *text)
        {
            TableModel* model = qobject_cast<TableModel*>(__this->model());
            model->setHeaderItem(column, QString::fromUtf8(text));
            __this->resizeColumnsToContents();
            __this->resizeRowsToContents();
        }

        void TableWidget_setItemElement(QTableView *__this, int row, int column, FormatElement *element)
        {
            TableModel* model = qobject_cast<TableModel*>(__this->model());
            model->setItem(row, column, element);
            __this->resizeColumnsToContents();
            __this->resizeRowsToContents();
        }

        void TableWidget_setItemString(QTableView *__this, int row, int column, const char *s)
        {
            TableModel* model = qobject_cast<TableModel*>(__this->model());
            model->setItem(row, column, QString::fromUtf8(s));
            __this->resizeColumnsToContents();
            __this->resizeRowsToContents();
        }
    }
}

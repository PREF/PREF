#ifndef PREFSDK_API_H
#define PREFSDK_API_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/ui/models/tablemodel.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            /* QWidget API */
            void Widget_resize(QWidget *__this, int w, int h);

            /* QLayout API */
            QLayout* Layout_createHorizontal();
            QLayout* Layout_createVertical();
            void BoxLayout_addLayout(QBoxLayout* __this, QLayout* layout, bool stretch);
            void Layout_addWidget(QLayout* __this, QWidget* widget);
            void Layout_setMargins(QLayout* __this, int left, int top, int right, int bottom);

            /* QDialog API */
            QDialog* Dialog_create(const char* title);
            void Dialog_setLayout(QDialog* __this, QLayout* layout);
            void Dialog_show(QDialog* __this);
            void Dialog_exec(QDialog* __this);

            /* QTableWidget API */
            QTableView *TableWidget_create(int rows, int columns);
            void TableWidget_setHeaderItem(QTableView *__this, int column, const char* text);
            void TableWidget_setItemElement(QTableView *__this, int row, int column, FormatElement* element);
            void TableWidget_setItemString(QTableView *__this, int row, int column, const char* s);

            /* QMessageBox API */
            void MessageBox_show(const char* title, const char* message);
        }
    }
}

#endif // PREFSDK_API_H

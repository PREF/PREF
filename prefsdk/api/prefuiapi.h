#ifndef PREFSDK_API_H
#define PREFSDK_API_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

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
            QTableWidget* TableWidget_create();
            void TableWidget_setColumnCount(QTableWidget* __this, int count);
            void TableWidget_setRowCount(QTableWidget* __this, int count);
            void TableWidget_setHeaderItem(QTableWidget *__this, int column, const char* text);
            void TableWidget_setItem(QTableWidget *__this, int row, int column, const char* text);
        }
    }
}

#endif // PREFSDK_API_H

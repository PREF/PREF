#ifndef PREFSDK_API_H
#define PREFSDK_API_H

#include "prefsdk/ui/tablewidget.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            /* View API */
            TableWidget* PrefUI_createTableWidget(const char* title);
            void PrefUI_show(QDialog* __this);
            void PrefUI_showModal(QDialog* __this);

            /* Table Widget API */
            void TableWidget_setColumnCount(TableWidget* __this, int count);
            void TableWidget_setRowCount(TableWidget* __this, int count);
            void TableWidget_setHeaderItem(TableWidget *__this, int column, const char* text);
            void TableWidget_setItem(TableWidget *__this, int row, int column, const char* text);
        }
    }
}

#endif // PREFSDK_API_H

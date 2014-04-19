#ifndef FORMATOPTIONSDIALOG_H
#define FORMATOPTIONSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <lua.hpp>
#include "viewmodels/optionmodel/optionmodel.h"
#include "prefsdk/format/formatlist.h"
#include "qhexedit/qhexedit.h"

using namespace PrefSDK;

namespace Ui {
class FormatOptionsDialog;
}

class FormatOptionsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FormatOptionsDialog(lua_State* l, const FormatList::Format& format, QHexEdit* hexedit, QWidget *parent = 0);
        ~FormatOptionsDialog();

    private:
        void validateFields();
        void executeOption(int optionidx);

    private slots:
        void on_tvOptions_doubleClicked(const QModelIndex &index);
        void on_tvOptions_clicked(const QModelIndex &index);
        void on_pbExecute_clicked();

    private:
        Ui::FormatOptionsDialog *ui;
        lua_State* _state;
        FormatList::Format _format;
        QHexEdit* _hexedit;
        int _selindex;
};

#endif // FORMATOPTIONSDIALOG_H

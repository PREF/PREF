#ifndef STRINGSTAB_H
#define STRINGSTAB_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <support/byteelaborator.h>
#include "../../models/stringsmodel.h"
#include "../../models/stringsmodel.h"

namespace Ui {
class StringsTab;
}

using namespace PrefLib::Support;

class StringsTab : public QWidget
{
    Q_OBJECT

    public:
        explicit StringsTab(QWidget *parent = 0);
        void initialize(QHexDocument *document);
        ~StringsTab();

    private slots:
        void on_tvStrings_doubleClicked(const QModelIndex &index);

    signals:
        void selectString(integer_t startoffset, integer_t endoffset);

    private:
        Ui::StringsTab *ui;
        StringsModel* _stringsmodel;
        QSortFilterProxyModel* _proxymodel;
        ByteElaborator::StringList _stringlist;
};

#endif // STRINGSTAB_H

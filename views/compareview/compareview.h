#ifndef COMPAREVIEW_H
#define COMPAREVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "views/abstractview.h"
#include "actionwidget/actiontoolbar.h"
#include "prefsdk/sdkmanager.h"

using namespace PrefSDK;

namespace Ui {
class CompareView;
}

class CompareView : public AbstractView
{
    Q_OBJECT

    public:
        explicit CompareView(QString leftfile, QString rightfile, const QString &viewname, QLabel* labelinfo, QWidget *parent = 0);
        ~CompareView();

    private slots:
        void compare();
        void updateLeftInfo(qint64);
        void updateRightInfo(qint64);

    private:
        void createToolbar(QHexEdit *hexedit, QWidget* tbcontainer, ActionWidget *actionwidget);

    public: /* Overriden Methods */
        virtual bool canSave() const;
        virtual void updateStatusBar();

    private:
        Ui::CompareView *ui;
        QHexEditData* _lefthexeditdata;
        QHexEditData* _righthexeditdata;
        QColor _diffcolor;
        QColor _samecolor;
};

#endif // COMPAREVIEW_H

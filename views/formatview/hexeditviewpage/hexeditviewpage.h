#ifndef HEXEDITVIEWPAGE_H
#define HEXEDITVIEWPAGE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "actionwidget/actionwidget.h"
#include "actionwidget/actiontoolbar.h"
#include "binarynavigator/binarynavigator.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "prefsdk/io/bytebuffer.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/sdkmanager.h"
#include "prefsdk/math.h"
#include "exportdialog.h"

using namespace PrefSDK;

namespace Ui {
class HexEditViewPage;
}

class HexEditViewPage: public QWidget
{
    Q_OBJECT

    public:
        explicit HexEditViewPage(QHexEditData* hexeditdata, QWidget *parent = 0);
        bool loadFormat(const FormatList::FormatId &formatid, int64_t baseoffset);
        void scanSignatures(bool canscan);
        BinaryNavigator* binaryNavigator();
        const FormatList::Format& format();
        FormatTree* tree();
        QHexEdit* hexEdit();
        ~HexEditViewPage();

    signals:
        void hexEditPositionChanged(qint64 pos);

    public slots:
        void gotoOffset(qint64 offset, qint64 length = -1);

    private:
        void createToolBar();
        void createHexEditContextMenu();

    private slots:
        void updateOffset(qint64 offset);
        void updateSelLength(qint64 size);
        void onHexEditCustomContextMenuRequested(const QPoint& pos);
        void onSetBackColor(FormatElement *formatlement);
        void onRemoveBackColor(FormatElement *formatlement);
        void onFormatObjectSelected(FormatElement* formatlement);
        void exportData(FormatElement* formatlement);
        void importData(FormatElement *formatlement);
        void scanSignatures();

    private:
        Ui::HexEditViewPage *ui;
        FormatList::Format _format;
        FormatModel* _formatmodel;
        FormatTree* _formattree;
        QHexEditData* _hexeditdata;
        ActionToolBar* _toolbar;
        QColor _signaturecolor;
        bool _signscanenabled;
        bool _entropyenabled;
};

#endif // HEXEDITVIEWPAGE_H

#ifndef HEXEDITVIEWPAGE_H
#define HEXEDITVIEWPAGE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "binarynavigator/binarynavigator.h"
#include "qhexedit/qhexedit.h"
#include "actionwidget/actionwidget.h"
#include "actionwidget/actiontoolbar.h"
#include "prefsdk/sdkmanager.h"
#include "prefsdk/io/bytebuffer.h"
#include "prefsdk/format/formatlist.h"
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
        explicit HexEditViewPage(ByteBuffer* bytebuffer, QWidget *parent = 0);
        void loadFormat(const FormatDefinition::Ptr& fd, qint64 baseoffset);
        void scanSignatures(bool canscan);
        BinaryNavigator* binaryNavigator();
        void setModel(FormatModel* model);
        FormatDefinition::Ptr formatDefinition();
        FormatModel* model();
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
        void onSetBackColor(FormatObject* formatobj);
        void onRemoveBackColor(FormatObject* formatobj);
        void onFormatObjectSelected(FormatObject *formatobj);
        void exportData(FormatObject* formatobj);
        void scanSignatures();

    private:
        Ui::HexEditViewPage *ui;
        FormatDefinition::Ptr _formatdefinition;
        FormatModel* _formatmodel;
        ByteBuffer* _bytebuffer;
        ActionToolBar* _toolbar;
        QColor _signaturecolor;
        bool _signscanenabled;
        bool _entropyenabled;
};

#endif // HEXEDITVIEWPAGE_H

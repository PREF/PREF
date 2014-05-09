#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "debugdialog/debugdialog.h"
#include "prefsdk/sdkmanager.h"
#include "views/hexview/hexview.h"
#include "views/compareview/compareview.h"
#include "views/compareview/comparedialog/comparedialog.h"
#include "signaturedatabasedialog/signaturedatabasedialog.h"
#include "aboutdialog.h"

using namespace PrefSDK;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        explicit MainWindow(QWidget *parent = 0);
        bool sdkLoaded();
        ~MainWindow();

    protected:
        virtual void closeEvent(QCloseEvent* e);

    private:
        void checkCommandLine();
        bool closeApplication();
        void enableMenuAndActions(bool b);
        void centerWindowToScreen();
        void adjustSplitterPosition();

    private slots:
        void loadFile(QString file);
        void on_action_Open_triggered();
        void on_action_Save_As_triggered();
        void on_action_Save_triggered();
        void on_actionExit_triggered();
        void on_actionCompare_triggered();
        void on_actionAbout_PREF_triggered();
        void on_tabWidget_tabCloseRequested(int index);
        void on_tabWidget_currentChanged(int index);
        void on_actionSignature_DB_triggered();
        void on_actionHex_File_triggered();

    private:
        Ui::MainWindow *ui;
        QLabel* _lblinfo;
        bool _sdkloaded;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "debugdialog/debugdialog.h"
#include "prefsdk/sdkmanager.h"
#include "views/hexview/hexview.h"
#include "views/disassemblerview/disassemblerview.h"
#include "views/disassemblerview/disassemblerdialog/disassemblerdialog.h"
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
        virtual void log(const QString& text);
        virtual void logLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);
        bool sdkLoaded();
        ~MainWindow();

    public:
        static MainWindow* instance();

    protected:
        virtual void closeEvent(QCloseEvent* e);

    private:
        void loadFile(QString file, QHexEditData *hexeditdata);
        void checkCommandLine();
        bool closeApplication();
        void setSaveVisible(bool b);
        void centerWindowToScreen();
        void adjustSplitterPosition();

    private slots:
        void loadFile(QString file);
        void on_action_Analyze_triggered();
        void on_action_Save_As_triggered();
        void on_action_Save_triggered();
        void on_actionExit_triggered();
        void on_actionCompare_triggered();
        void on_actionAbout_PREF_triggered();
        void on_tabWidget_tabCloseRequested(int index);
        void on_tabWidget_currentChanged(int index);
        void on_actionSignature_DB_triggered();
        void on_actionHex_File_triggered();
        void on_actionDisassemble_triggered();

    private:
        static MainWindow* _instance;
        Ui::MainWindow *ui;
        QLabel* _lblinfo;
        bool _sdkloaded;
};

#endif // MAINWINDOW_H

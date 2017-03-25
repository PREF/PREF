#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/views/abstractview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_action_Analyze_triggered();

    private:
        void centerWindow();
        void updateToolBar(AbstractView* abstractview) const;

    private:
        Ui::MainWindow *ui;
        AbstractView* _currentview;
        QLabel* _lblstatus;
};

#endif // MAINWINDOW_H

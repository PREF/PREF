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

    protected:
        virtual void dragEnterEvent(QDragEnterEvent* e);
        virtual void dragMoveEvent(QDragMoveEvent* e);
        virtual void dropEvent(QDropEvent* e);
        virtual void closeEvent(QCloseEvent* e);

    private slots:
        void on_action_Analyze_triggered();

    private:
        void updateToolBar(AbstractView* abstractview) const;
        void centerWindow();
        void loadFile(const QString& file);
        bool closeApplication() const;

    private:
        Ui::MainWindow *ui;
        AbstractView* _currentview;
        QLabel* _lblstatus;
};

#endif // MAINWINDOW_H

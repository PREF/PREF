#ifndef NUMERICBASEMENU_H
#define NUMERICBASEMENU_H

#include <QMenu>

class NumericBaseMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit NumericBaseMenu(QWidget *parent = 0);
        void setBase(int base);

    private slots:
        void onActionBinaryTriggered();
        void onActionOctalTriggered();
        void onActionDecimalTriggered();
        void onActionHexadecimalTriggered();

    signals:
        void baseChanged(int base);

    private:
        QAction* _actbinary;
        QAction* _actoctal;
        QAction* _actdecimal;
        QAction* _acthexadecimal;
        QActionGroup* _actgroup;
};

#endif // NUMERICBASEMENU_H

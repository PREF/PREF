#ifndef DATATYPESMENU_H
#define DATATYPESMENU_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "numericbasemenu.h"

class DataTypesMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit DataTypesMenu(QWidget *parent = 0);
        void setBase(int b);
        void setEndian(QSysInfo::Endian endian);

    signals:
        void endianChanged(QSysInfo::Endian endian);
        void baseChanged(int base);

    public slots:
        void onLittleEndianTriggered();
        void onBigEndianTriggered();

    private:
        QMenu* _endianmenu;
        QActionGroup* _endiangroup;
        QAction* _actlittleendian;
        QAction* _actbigendian;
        NumericBaseMenu* _numbasemenu;
};

#endif // DATATYPESMENU_H

#ifndef COPYMENU_H
#define COPYMENU_H

#include <QMenu>

class CopyMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit CopyMenu(QWidget *parent = 0);
        void setCopyValueVisible(bool b);

    signals:
        void copyOffset();
        void copyName();
        void copyValue();

    private:
        QAction* _copyoffset;
        QAction* _copyname;
        QAction* _copyvalue;
};

#endif // COPYMENU_H

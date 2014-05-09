#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(QStatusBar* statusbar, QWidget *parent = 0);
        virtual ~AbstractView();

    public: /* Abstract Methods */
        virtual bool canSave() const = 0;
        virtual void updateStatusBar() = 0;

    protected:
        QStatusBar* _statusbar;
};

#endif // ABSTRACTVIEW_H

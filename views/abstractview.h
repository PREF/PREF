#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "logwidget/logwidget.h"

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(const QString& viewname, QLabel *labelinfo, QWidget *parent = 0);
        const QString& viewName() const;
        virtual ~AbstractView();

    protected:
        void updateInfoText(const QString& s);

    public: /* Abstract Methods */
        virtual bool canSave() const = 0;
        virtual void updateStatusBar() = 0;
        virtual void log(const QString& text) = 0;
        virtual void logLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing) = 0;

    private:
        QLabel* _lblinfo;
        QString _viewname;
};

#endif // ABSTRACTVIEW_H

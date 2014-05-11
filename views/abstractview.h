#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(const QString& viewname, QLabel *labelinfo, QWidget *parent = 0);
        virtual ~AbstractView();

    private:
        const QString& viewName() const;

    protected:
        void updateInfoText(const QString& s);

    public: /* Abstract Methods */
        virtual bool canSave() const = 0;
        virtual void updateStatusBar() = 0;

    private:
        QLabel* _lblinfo;
        QString _viewname;
};

#endif // ABSTRACTVIEW_H

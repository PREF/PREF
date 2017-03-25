#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include "qhexedit/qhexeditdata.h"

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(QHexEditData *hexeditdata, QLabel* lblstatus, const QString& loadedfile, QWidget *parent = 0);
        virtual ~AbstractView();
        const QString& loadedFile() const;
        virtual void updateToolBar(QToolBar* toolbar) const;

    protected:
        QHexEditData* _hexeditdata;
        QLabel* _lblstatus;

    private:
        QString _loadedfile;
};

#endif // ABSTRACTVIEW_H

#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include "qhexedit/qhexeditdata.h"
#include <QWidget>
#include <QLabel>

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(QHexEditData *hexeditdata, const QString& viewname, QLabel *labelinfo, QWidget *parent = 0);
        virtual ~AbstractView();
        virtual bool canSaveAs() const;
        virtual QString saveFilter() const;
        virtual void save(const QString&, const QString&);
        virtual void save();
        const QString& viewName() const;

    protected:
        void updateInfoText(const QString& s);

    public: /* Abstract Methods */
        virtual QHexEditData* data() = 0;
        virtual bool canSave() const = 0;
        virtual void updateStatusBar() = 0;

    protected:
        QLabel* _lblinfo;
        QHexEditData* _hexeditdata;

    private:
        QString _viewname;
};

#endif // ABSTRACTVIEW_H

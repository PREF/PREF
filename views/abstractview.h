#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include "qhexedit/qhexeditdata.h"
#include "logwidget/logwidget.h"
#include <QWidget>
#include <QLabel>

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(QHexEditData *hexeditdata, const QString& loadedfile, QLabel *labelinfo, QWidget *parent = 0);
        virtual ~AbstractView();
        virtual bool canSaveAs() const;
        virtual QString saveFilter() const;
        virtual void saveAs();
        virtual void save();
        const QString& loadedFile() const;

    protected:
        void updateInfoText(const QString& s);

    public: /* Abstract Methods */
        virtual QHexEditData* data() = 0;
        virtual bool canSave() const = 0;
        virtual void updateStatusBar() = 0;
        virtual LogWidget* logWidget() = 0;

    protected:
        QLabel* _lblinfo;
        QHexEditData* _hexeditdata;

    private:
        QString _loadedfile;
};

#endif // ABSTRACTVIEW_H

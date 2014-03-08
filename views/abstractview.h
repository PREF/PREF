#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>

class AbstractView : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractView(QWidget *parent = 0);
        virtual ~AbstractView();

    public: /* Abstract Methods */
        virtual bool canSave() const = 0;
};

#endif // ABSTRACTVIEW_H

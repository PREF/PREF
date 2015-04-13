#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <disassembler/blocks/block.h>

using namespace PrefLib::Disassembler;

namespace Ui {
class BookmarkWidget;
}

class BookmarkWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit BookmarkWidget(QWidget *parent = 0);
        void setBlock(Block* block);
        ~BookmarkWidget();

    signals:
        void saveBookmarkRequested(Block* block, QString description);

    private slots:
        void doSaveRequest();

    protected:
        virtual void hideEvent(QHideEvent* e);
        virtual void showEvent(QShowEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        Ui::BookmarkWidget *ui;
        Block* _block;
};

#endif // BOOKMARKWIDGET_H

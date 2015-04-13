#ifndef RENAMEWIDGET_H
#define RENAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include "disassembler/blocks/block.h"

using namespace PrefLib::Disassembler;

namespace Ui {
class RenameWidget;
}

class RenameWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit RenameWidget(QWidget *parent = 0);
        void setLabel(const QString &name);
        void setBlock(Block* block);
        ~RenameWidget();

    signals:
        void renameRequested(Block* block, QString description);

    private slots:
        void doRenameRequest();

    protected:
        virtual void hideEvent(QHideEvent* e);
        virtual void showEvent(QShowEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        Ui::RenameWidget *ui;
        Block* _block;
};

#endif // RENAMEWIDGET_H

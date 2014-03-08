#include "preftabwidget.h"

PrefTabWidget::PrefTabWidget(QWidget *parent): QTabWidget(parent)
{
    this->setAcceptDrops(true);
}

void PrefTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void PrefTabWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void PrefTabWidget::dropEvent(QDropEvent *event)
{
    bool accept = false;
    const QMimeData* mimedata = event->mimeData();

    if(mimedata->hasUrls())
    {
        QList<QUrl> urllist = mimedata->urls();

        for(int i = 0; (i < urllist.length()) && (i < 10); i++)
        {
            QString locfile = urllist[i].toLocalFile();
            QFileInfo fi(locfile);

            if(fi.isFile())
            {
                accept = true;
                emit fileDragged(urllist[i].toLocalFile());
            }
        }

        if(accept)
            event->acceptProposedAction();
    }
}

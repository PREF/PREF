#include "debugapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Debug_log(QHexEditData *hexeditdata, const char *s)
        {
            AbstractView* view = MainWindow::instance()->loadedView(hexeditdata);
            view->log(QString::fromUtf8(s));
        }

        void Debug_logNotice(QHexEditData *hexeditdata, const char *s)
        {
            AbstractView* view = MainWindow::instance()->loadedView(hexeditdata);
            view->logLine(QString::fromUtf8(s), LogWidget::Notice);
        }

        void Debug_logWarning(QHexEditData *hexeditdata, const char *s)
        {
            AbstractView* view = MainWindow::instance()->loadedView(hexeditdata);
            view->logLine(QString::fromUtf8(s), LogWidget::Warning);
        }

        void Debug_logError(QHexEditData *hexeditdata, const char *s)
        {
            AbstractView* view = MainWindow::instance()->loadedView(hexeditdata);
            view->logLine(QString::fromUtf8(s), LogWidget::Error);
        }

        void Debug_showDialog(const char *s)
        {
            DebugDialog::instance()->out(QString::fromUtf8(s));

            if(!DebugDialog::instance()->isVisible())
                DebugDialog::instance()->exec();
        }
    }
}

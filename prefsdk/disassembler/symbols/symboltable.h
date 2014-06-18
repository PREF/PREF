#ifndef PREFSDK_SYMBOLTABLE_H
#define PREFSDK_SYMBOLTABLE_H

#include <QObject>

namespace PrefSDK {

class SymbolTable : public QObject
{
    Q_OBJECT
public:
    explicit SymbolTable(QObject *parent = 0);

signals:

public slots:

};

} // namespace PrefSDK

#endif // PREFSDK_SYMBOLTABLE_H

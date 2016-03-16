#ifndef QTCACHEBULKEXPORT_H
#define QTCACHEBULKEXPORT_H

#include "qtcachebulkaction.h"
#include <QDir>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT BulkExport
        : public BulkAction
{
    Q_OBJECT

public:
    QDir outputDirectory;
    QString filter;
    QtCache::ObjectFilterType filterType;
    QStringList contentFilter;

    explicit BulkExport(QtCache* cache, QObject *parent = 0);

public slots:
    void run();

private:
    void save(const QDir& outputDirectory, const QString& filter, QtCache::ObjectFilterType filterType, const QStringList& contentFilter);
};

QTCACHENAMESPACEEND

#endif // QTCACHEBULKEXPORT_H

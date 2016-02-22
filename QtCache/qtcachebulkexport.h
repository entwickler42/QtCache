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
    explicit BulkExport(QtCache* cache, QObject *parent = 0);

public slots:
    void save(const QDir& outputDirectory, const QString& filter, QtCache::ObjectFilterType filterType);
};

QTCACHENAMESPACEEND

#endif // QTCACHEBULKEXPORT_H

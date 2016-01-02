#ifndef BULKIMPORTPROGRESS_H
#define BULKIMPORTPROGRESS_H

#include "qtcache_global.h"
#include <QString>

QTCACHENAMESPACEBEGIN

class BulkImportProgress
{
public:
    enum ImportStep
    {
        IDLE,
        READING,
        UPLOADING,
        COMPILING
    };

    BulkImportProgress(ImportStep step, const QString& sourceName, int pos, int max)
        : step(step),
          objectName(sourceName),
          sourceName(sourceName),
          pos(pos),
          max(max)
    {}

    BulkImportProgress(ImportStep step, const QString& objectName, const QString& sourceName, int pos, int max)
        : step(step),
          objectName(objectName),
          sourceName(sourceName),
          pos(pos),
          max(max)
    {}

    ImportStep step = IDLE;
    QString objectName;
    QString sourceName;
    int pos;
    int max;
};

QTCACHENAMESPACEEND

#endif // BULKIMPORTPROGRESS_H


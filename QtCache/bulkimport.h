#ifndef BULKIMPORT_H
#define BULKIMPORT_H

#include <QObject>
#include "qtcache_global.h"
#include "qtcache.h"

namespace LibQtCache {

class QTCACHESHARED_EXPORT BulkImport
        : public QObject
{
    Q_OBJECT

public:
    explicit BulkImport(QtCache* cache, QObject *parent = 0);

signals:
    void finished();
    void error(std::exception& ex);
    void loading(const QString& filename, int pos, int max);
    void uploading(const QString& filename, int pos, int max);
    void compiling(const QString& filename, int pos, int max);

public slots:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void abort()
    {
        m_abort_import = true;
    }

private:
    QtCache* m_cache = NULL;
    bool m_abort_import = false;
};

}

#endif // BULKIMPORT_H
